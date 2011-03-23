/*
   OrionSocket - Util
   --------------------------------

   Author: Tiago Natel de Moura <tiago4orion@gmail.com>

   Copyright 2010, 2011 by Tiago Natel de Moura. All Rights Reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 */
#ifndef __ORIONSOCKET_UTIL_H_
#define __ORIONSOCKET_UTIL_H_

#include "types.h"
#include <stdio.h>

extern void * orion_realloc(void *ptr, size_t size);
extern const char* orion_getStrMethod(_u8 method);
extern _i32 orion_linearSearchChar(const char* buffer, char c);
extern char* orion_getNextLine(char* buffer);
extern char* orion_ltrim(char* buffer);

#endif /* __ORIONSOCKET_UTIL_H_ */

