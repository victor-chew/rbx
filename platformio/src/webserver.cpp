/*
 * webserver.cpp
 *
 * Copyright 2020 Victor Chew
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include "microtar.h"
#include "javascript.h"

AsyncWebServer www(80);
AsyncEventSource events("/events");

// 'C' = Control
// 'E' = Error
// 'F' = Filename
// 'M' = Message from program
// 'S' = Status
void logger(char type, const char* fmt, ...) {
  char cfmt[128], msg[256];
  sprintf(cfmt, "%c:%s", type, fmt);
  va_list ap;
  va_start(ap, fmt);
  vsprintf(msg, cfmt, ap);
  va_end(ap);
  Serial.println(msg);
  events.send(msg, NULL, millis(), 1000);
}

int cstring_cmp(const void *p1, const void *p2) {
  const char **s1 = (const char **)p1;
  const char **s2 = (const char **)p2;
  return strcmp(*s1, *s2);
}

void sendProgList() {
  // Get name of autorun script
  String autorun = getJsAutorun();
  // Indicates start of script listing
  logger('F', "/rbx");
  // Send script names
  File root = SPIFFS.open("/rbx", FILE_READ);
  File file = root.openNextFile();
  while(file) {
    if (*(file.name()+5) != '.') {
      String scriptName = String(file.name() + 5);
      if (strcmp(file.name(), autorun.c_str()) == 0)
        scriptName = String("*") + scriptName; // Leading asterisk indicates autorun script
      logger('F', scriptName.c_str());
    }
    file = root.openNextFile(); 
  }
  file.close();
  root.close();
}

String sanitizeFilename(String filename) {
  String result;
  for (int i=0; i<filename.length(); i++) {
    if (i == 0 && filename[i] == '.') continue;
    result += (filename[i] != '/' ? filename[i] : '_');
  }
  return result.substring(0, 31-5); // less 5 chars for "/rbx/"
}

void saveProg(AsyncWebServerRequest* request, String name, String script, bool autorun) {
  // Save script
  String fullname =  String("/rbx/") + name;
  File file = SPIFFS.open(fullname, FILE_WRITE);
  if (!file) {
    logger('E', "*** OPEN ERROR: %s", name);
    request->send(500, "text/plain", "Error");
  }
  int written = file.print(script);
  if (written != script.length()) {
    file.close();
    logger('E', "*** WRITE ERROR: %s", name.c_str());
    request->send(500, "text/plain", "Error");
  }
  file.close();
  // Handle autorun
  if (autorun) setJsAutorun(fullname);
  else if (getJsAutorun() == fullname) setJsAutorun("");
  // Reply success
  logger('S', "Saved: %s", name.c_str());
  sendProgList();
  request->send(200, "text/plain", "OK");
}

void createBackup() {
  mtar_t tar;
  mtar_open(&tar, "/backup.tar", "w");
  File root = SPIFFS.open("/rbx", FILE_READ);
  File file = root.openNextFile();
  while(file) { 
    File curfile = SPIFFS.open(file.name(), FILE_READ);
    String script; while(curfile.available()) script += (char)curfile.read();
    curfile.close();
    mtar_write_file_header(&tar, file.name(), strlen(script.c_str()));
    mtar_write_data(&tar, script.c_str(), strlen(script.c_str()));
    file = root.openNextFile(); 
  }
  file.close();
  root.close();
  mtar_finalize(&tar);
  mtar_close(&tar);
}

void restoreBackup() {
  char *data;
  mtar_t tar;
  mtar_header_t hdr;
  mtar_open(&tar, "/backup.tar", "r");
  while ( (mtar_read_header(&tar, &hdr)) != MTAR_ENULLRECORD ) {
    Serial.println(hdr.name);
    if (strncmp(hdr.name, "/rbx/", 5) != 0) continue;
    data = (char*)calloc(1, hdr.size + 1);
    mtar_read_data(&tar, data, hdr.size);
    File file = SPIFFS.open(hdr.name, FILE_WRITE);
    file.write((const unsigned char*)data, hdr.size);
    file.close();
    free(data);
    logger('S', "Restored: %s", hdr.name+5);
    mtar_next(&tar);
  }
  mtar_close(&tar);
}

void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
  File file = SPIFFS.open("/backup.tar", "a");
  file.write((const unsigned char*)data, len);
  file.close();
  if (!final) return;
  restoreBackup();
  sendProgList();
}

void initWebServer() {
    www.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });
    www.on("/help.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/help.html", "text/html");
    });
    www.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style.css", "text/css");
    });
    www.on("/rbx.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/rbx.js", "text/javascript");
    });
    www.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/favicon.ico", "image/vnd.microsoft.icon");
    });
    www.on("/rbx.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/favicon.ico", "image/png");
    });
    www.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request) {
        stopJavascript();
        request->send(200, "text/plain", "OK");
    });
    www.on("/list", HTTP_GET, [](AsyncWebServerRequest *request) {
      sendProgList();
      request->send(200, "text/plain", "OK");
    });
    www.on("/run", HTTP_POST, [](AsyncWebServerRequest *request) {
      if (request->hasParam("script", true)) {
        const char* script = request->getParam("script", true)->value().c_str();
        Serial.println(script);
        runJavascript(script);
      }
      request->send(200, "text/plain", "OK");
    });
    www.on("/backup", HTTP_GET, [](AsyncWebServerRequest *request) {
      createBackup();
      request->send(SPIFFS, "/backup.tar", String(), true);
    });
    www.on("/restore", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200);
    }, handleUpload);
    www.addHandler(new AsyncCallbackJsonWebHandler("/open", 
      [](AsyncWebServerRequest* request, JsonVariant& json) {
        JsonObject& jsonObj = json.as<JsonObject>();
        String name = jsonObj["name"];
        String fullname =  String("/rbx/") + name;
        File file = SPIFFS.open(fullname, FILE_READ);
        if (!file) {
          logger('E', "OPEN ERROR: %s", name);
          request->send(500, "text/plain", "Error");
        }
        String script; while(file.available()) script += (char)file.read();
        file.close();
        request->send(200, "text/plain", script.c_str());
      }
    ));
    www.addHandler(new AsyncCallbackJsonWebHandler("/save", 
      [](AsyncWebServerRequest* request, JsonVariant& json) {
        JsonObject& jsonObj = json.as<JsonObject>();
        String name = sanitizeFilename(jsonObj["name"]);
        String script = jsonObj["script"];
        bool autorun = jsonObj["autorun"] == "true";
        saveProg(request, name, script, autorun);
      }
    ));
    www.addHandler(new AsyncCallbackJsonWebHandler("/delete", 
      [](AsyncWebServerRequest* request, JsonVariant& json) {
        JsonObject& jsonObj = json.as<JsonObject>();
        String name = sanitizeFilename(jsonObj["name"]);
        String fullname =  String("/rbx/") + name;
        bool success = SPIFFS.remove(fullname);
        if (!success) {
          logger('E', "*** DELETE ERROR: %s", name);
          request->send(500, "text/plain", "Error");
        }
        logger('S', "Deleted: %s", name.c_str());
        sendProgList();
        request->send(200, "text/plain", "OK");
      }
    ));
    www.addHandler(new AsyncCallbackJsonWebHandler("/key", 
      [](AsyncWebServerRequest* request, JsonVariant& json) {
        JsonObject& jsonObj = json.as<JsonObject>();
        String key = jsonObj["key"];
        pushJsKey(key);
        request->send(200, "text/plain", "OK");
      }
    ));
    events.onConnect([](AsyncEventSourceClient *client) {
      sendProgList();
      logger('C', isScriptRunning() ? "RUNNING" : "READY");
    });
    www.addHandler(&events);
    www.begin();
}