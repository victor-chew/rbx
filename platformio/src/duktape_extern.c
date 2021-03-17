/*
 * duktape_extern.c
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
#include "duktape.h"

extern bool pendingJsTermination();

// These functions are used by duk_config.h

duk_bool_t rbx_timeout_check(void *udata) {
    if (pendingJsTermination()) return 1;
    vTaskDelay(pdMS_TO_TICKS(1)); // Must call periodically to prevent watchdog timeout
    return 0;
}

duk_double_t rbx_get_now() {
  return millis();
}

// Need our own realloc() function because passing ptr=NULL or size=0 to ESP32's own realloc()
// will need to heap assertion failure
void* rbx_realloc(void *ptr, size_t size) {
  if (size == 0) return NULL;
  else if (ptr == NULL) return malloc(size);
  return realloc(ptr, size);
}
