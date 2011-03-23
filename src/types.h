/*
   OrionSocket - Types
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
 
#ifndef __ORIONSOCKET_TYPES_H_
#define __ORIONSOCKET_TYPES_H_

#include <stdint.h>

/* Unsigned types */
typedef uint8_t _u8;
typedef uint16_t _u16;
typedef uint32_t _u32;

typedef int8_t _i8;
typedef int16_t _i16;
typedef int32_t _i32;

/**
 * Structure Name-Value. \n
 * Used in various parts of the library
 */
typedef struct 
{
    char* name; 
    char* value; 
} nameValue;

#endif /* __ORIONSOCKET_TYPES_H_ */

