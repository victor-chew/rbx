/*
 * main.cpp
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

#include <stdarg.h>
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <rbx.h>
#include "javascript.h"
#include "webserver.h"

OledDisplay screen;

void showInfo(OledDisplay& screen, const char* title, const char* msg) {
  screen.clear();
  int sw = screen.stringWidth(msg);
  int sh = screen.stringHeight(msg);
  int sx = (screen.width() - sw) / 2;
  int sy = ((screen.height() - (sh*3)) / 2) + sh;
  screen.drawString(sx, sy, title);
  screen.drawString(sx, sy + (sh*2), msg);
}

void setup() {
  Serial.begin(115200);
  if (!SPIFFS.begin(true)) Serial.println("SPIFFS cannot be mounted.");

  Wire.begin();

  // Display SSID on OLED display
  uint32_t chipId = 0;
  for(int i=0; i<17; i=i+8) chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  String ssid = String("RBX-") + String(chipId);
  Serial.print("SSID: "); Serial.println(ssid);  
  showInfo(screen, "Setup:", ssid.c_str());

  // Connect to router, then initialize web server and Javascript runtime
  OTA_Init(ssid.c_str());
  initWebServer();
  initJavascript();

  // Display IP address on screen
  IPAddress ipaddr = WiFi.localIP(); 
  showInfo(screen, "Connected:", ipaddr.toString().c_str());

  // Start autorun script (if any)
  autorunJavascript();
}

void loop() {
  vTaskDelete(NULL);
}
