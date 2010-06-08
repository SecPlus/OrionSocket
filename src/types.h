/*
   OrionSocket - Types
   --------------------------------

   Author: Tiago Natel de Moura <tiago4orion@gmail.com>

   Copyright 2007, 2008 by Tiago Natel de Moura. All Rights Reserved.

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
 
#ifndef __ORIONSOCKET_TYPES_H_
#define __ORIONSOCKET_TYPES_H_

#include <stdint.h>

typedef uint8_t _uint8;
typedef uint16_t _uint16;
typedef uint32_t _uint32;

// Structure Name-Value.
typedef struct 
{
    char* name;
    char* value;
} nameValue;

#endif // __ORIONSOCKET_TYPES_H_  

