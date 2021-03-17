/*
 * javascript.cpp
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

#include <queue>
#include <Arduino.h>
#include <SPIFFS.h>
#include <rbx.h>
#include "duktape.h"
#include "webserver.h"

void RBX_init(duk_context* ctx);

TaskHandle_t hJsTask;
char* jssource; 
bool jsrunning, jsterminate;
std::queue<String> keys;
Keystore* keystore;

void js_fatal_error(void *udata, const char *msg) {
    char text[256];
    sprintf(text, "*** FATAL ERROR: %s", msg);
    logger('E', text);
    logger('C', "STOP");
}

duk_ret_t js_print(duk_context* ctx) {
	duk_push_string(ctx, " ");
	duk_insert(ctx, 0);
	duk_join(ctx, duk_get_top(ctx) - 1);
	logger('M', duk_safe_to_string(ctx, -1));
	return 0;
}

duk_ret_t js_freemem(duk_context* ctx) {
	duk_push_number(ctx, ESP.getFreeHeap());
	return 1;
}

duk_ret_t js_version(duk_context* ctx) {
	duk_push_string(ctx, "RBX Javascript Runtime v0.1.1");
	return 1;
}

duk_ret_t js_getkey(duk_context* ctx) {
  if (keys.empty()) {
    duk_push_null(ctx);
  }
  else {
    String key = keys.front();
    keys.pop();
  	duk_push_string(ctx, key.c_str());
  }
	return 1;
}

void pushJsKey(String key) {
  keys.push(key);
}

// void Keystore.load()
duk_ret_t Keystore_load(duk_context* ctx) {
  keystore->load();
	return 0;
}

// void Keystore.save()
duk_ret_t Keystore_save(duk_context* ctx) {
  keystore->save();
	return 0;
}

// int Keystore.count()
duk_ret_t Keystore_count(duk_context* ctx) {
  int result = keystore->count();
  duk_push_number(ctx, result);
  return 1;
}

// bool Keystore.exists(key)
duk_ret_t Keystore_exists(duk_context* ctx) {
	if (duk_get_top(ctx) < 1) {
		duk_push_error_object(ctx, DUK_ERR_TYPE_ERROR, "exists(key)");
		return duk_throw(ctx);
	}
	const char* key = (const char*)duk_require_string(ctx, 0);
  bool result = keystore->exists(key);
  duk_push_boolean(ctx, result);
  return 1;
}

// String  Keystore.getValue(key)
duk_ret_t Keystore_getValue(duk_context* ctx) {
	if (duk_get_top(ctx) < 1) {
		duk_push_error_object(ctx, DUK_ERR_TYPE_ERROR, "getValue(key)");
		return duk_throw(ctx);
	}
	const char* key = (const char*)duk_require_string(ctx, 0);
  String result = keystore->getValue(key);
  duk_push_string(ctx, result.c_str());
  return 1;
}

// int Keystore.getKeyByIndex(index)
duk_ret_t Keystore_getKeyByIndex(duk_context* ctx) {
	if (duk_get_top(ctx) < 1) {
		duk_push_error_object(ctx, DUK_ERR_TYPE_ERROR, "getKeyByIndex(index)");
		return duk_throw(ctx);
	}
	int index = (int)duk_require_number(ctx, 0);
  String result = keystore->getKeyByIndex(index);
  duk_push_string(ctx, result.c_str());
  return 1;
}

// int Keystore.getValueByIndex(index)
duk_ret_t Keystore_getValueByIndex(duk_context* ctx) {
	if (duk_get_top(ctx) < 1) {
		duk_push_error_object(ctx, DUK_ERR_TYPE_ERROR, "getValueByIndex(index)");
		return duk_throw(ctx);
	}
	int index = (int)duk_require_number(ctx, 0);
  String result = keystore->getValueByIndex(index);
  duk_push_string(ctx, result.c_str());
  return 1;
}

// void Keystore.setValue(key, value)
duk_ret_t Keystore_setValue(duk_context* ctx) {
	if (duk_get_top(ctx) < 2) {
		duk_push_error_object(ctx, DUK_ERR_TYPE_ERROR, "setValue(key, value)");
		return duk_throw(ctx);
	}
	const char* key = (const char*)duk_require_string(ctx, 0);
	const char* value = (const char*)duk_require_string(ctx, 1);
  keystore->setValue(key, value);
	return 0;
}

// void Keystore.remove(key)
duk_ret_t Keystore_remove(duk_context* ctx) {
	if (duk_get_top(ctx) < 1) {
		duk_push_error_object(ctx, DUK_ERR_TYPE_ERROR, "remove(key)");
		return duk_throw(ctx);
	}
	const char* key = (const char*)duk_require_string(ctx, 0);
  keystore->remove(key);
	return 0;
}

// void Keystore.clear()
duk_ret_t Keystore_clear(duk_context* ctx) {
  keystore->clear();
	return 0;
}

void Keystore_init(duk_context* ctx) {
	duk_push_object(ctx);
	duk_push_c_function(ctx, Keystore_load, 0);
	duk_put_prop_string(ctx, -2, "load");
	duk_push_c_function(ctx, Keystore_save, 0);
	duk_put_prop_string(ctx, -2, "save");
	duk_push_c_function(ctx, Keystore_count, 0);
	duk_put_prop_string(ctx, -2, "count");
	duk_push_c_function(ctx, Keystore_exists, DUK_VARARGS);
	duk_put_prop_string(ctx, -2, "exists");
	duk_push_c_function(ctx, Keystore_getValue, DUK_VARARGS);
	duk_put_prop_string(ctx, -2, "getValue");
	duk_push_c_function(ctx, Keystore_getKeyByIndex, DUK_VARARGS);
	duk_put_prop_string(ctx, -2, "getKeyByIndex");
	duk_push_c_function(ctx, Keystore_getValueByIndex, DUK_VARARGS);
	duk_put_prop_string(ctx, -2, "getValueByIndex");
	duk_push_c_function(ctx, Keystore_setValue, DUK_VARARGS);
	duk_put_prop_string(ctx, -2, "setValue");
	duk_push_c_function(ctx, Keystore_remove, DUK_VARARGS);
	duk_put_prop_string(ctx, -2, "remove");
	duk_push_c_function(ctx, Keystore_clear, 0);
	duk_put_prop_string(ctx, -2, "clear");
	duk_put_global_string(ctx, "Keystore_prototype");
}

duk_ret_t js_getkeystore(duk_context* ctx) {
  duk_push_object(ctx);
  duk_get_global_string(ctx, "Keystore_prototype");
  duk_set_prototype(ctx, -2); 
	return 1;
}

void initJavascript(duk_context* ctx) {
  keys = std::queue<String>();
  keystore = Keystore::getInstance();
  // Keystore prototype
  Keystore_init(ctx);
	// Utility functions in C++
	duk_push_c_function(ctx, js_print, DUK_VARARGS); duk_put_global_string(ctx, "print");
	duk_push_c_function(ctx, js_freemem, 0); duk_put_global_string(ctx, "freemem");
	duk_push_c_function(ctx, js_version, 0); duk_put_global_string(ctx, "version");
  duk_push_c_function(ctx, js_getkey, 0); duk_put_global_string(ctx, "getkey");
  duk_push_c_function(ctx, js_getkeystore, 0); duk_put_global_string(ctx, "getkeystore");
  // delay() function is coded in Javascript
  const char* delayFunc = "function delay(interval_ms) {\n"
    "var last = Date.now();\n"
    "while(true) {\n"
      "var now = Date.now();\n"
      "if (now - last >= interval_ms) break;\n"
  "}}\n";
  duk_push_string(ctx, delayFunc); duk_peval(ctx); duk_pop(ctx);  
  // random() function is coded in Javascript
  const char* randomFunc = "function random(min, max) {\n"
    "return Math.floor(min + (Math.random() * (max - min + 1)));\n"
  "}\n";
  duk_push_string(ctx, randomFunc); duk_peval(ctx); duk_pop(ctx);  
}

void runJavascript(const char* src) {
    if (jsrunning) return;
    jssource = strdup(src);
    xTaskNotifyGive(hJsTask);
}

void stopJavascript() {
    if (!jsrunning || jsterminate) return;
    jsterminate = true; 
}

bool isScriptRunning() {
  return jsrunning;
}

extern "C" bool pendingJsTermination() {
  return jsterminate;
}

void javascriptTask(void* param) {
    while(true) {
      ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
      duk_context* ctx = duk_create_heap(NULL, NULL, NULL, NULL, js_fatal_error);
      initJavascript(ctx);
      RBX_init(ctx);
      jsrunning = true;
      logger('C', "START");
      if (duk_pcompile_string(ctx, 0, jssource) != 0) {
        logger('E', duk_safe_to_string(ctx, -1));
      }
      else {
        if (duk_pcall(ctx, 0) != DUK_EXEC_SUCCESS) {
          if (!jsterminate) {
            duk_get_prop_string(ctx, -1, "stack"); 
            logger('E', duk_safe_to_string(ctx, -1));
            logger('S', "Ready");
          }
          else logger('E', "*** TERMINATED BY USER");
        }
        duk_pop(ctx);
      }
      jsrunning = false;
      jsterminate = false;
      logger('C', "STOP");
      duk_destroy_heap(ctx);
      free(jssource);
    }
}

void initJavascript() {
    xTaskCreatePinnedToCore(javascriptTask, "jsTask", 8192, NULL, 1, &hJsTask, 0);
}

#define AUTORUN "/rbx/.autorun"

String getJsAutorun() {
  File autorun = SPIFFS.open(AUTORUN, FILE_READ);
  if (autorun) {
    String scriptName; 
    while(autorun.available()) scriptName += (char)autorun.read();
    autorun.close();
    return scriptName;
  }
  return String();
} 

void setJsAutorun(String scriptName) {
  SPIFFS.remove(AUTORUN);
  if (scriptName.length() == 0) return;
  File autorun = SPIFFS.open(AUTORUN, FILE_WRITE);
  if (autorun) {
    autorun.print(scriptName);
    autorun.close();
  }
} 

void autorunJavascript() {
  String scriptName = getJsAutorun();
  if (scriptName.length() > 0) {
    File autorun = SPIFFS.open(scriptName, FILE_READ);
    if (autorun) {
      String scriptJs;
      while(autorun.available()) scriptJs += (char)autorun.read();
      autorun.close();
      runJavascript(scriptJs.c_str());
    }
  }
}
