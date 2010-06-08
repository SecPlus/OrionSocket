/*
   OrionSocket - HTTP Request and Response handling
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
#ifndef __ORIONSOCKET_HTTP_H_
#define __ORIONSOCKET_HTTP_H_

#include "types.h"

// orionHttpRequest structure
typedef struct
{
    char* host;         /* Host Target          */
    _uint16 port;       /* Port Target          */
    _uint8 method;      /* HTTP Method          */
    char* path;         /* URL Path             */
    char* file_ext;     /* File extension       */
    char* query;        /* Query String         */
    
    nameValue *header;  /* HTTP    Headers      */
    _uint8 headerLen;   /* Number of headers    */
    
    nameValue *cookie;  /* Array of cookies     */
    _uint8 cookieLen;   /* Number of cookies    */
    
    _uint16 options;    /* Extra Options        */
} orionHttpRequest;

typedef struct
{
    _uint8 code;        /* Status Code          */
    char* file_ext;     /* File extension       */
    
    nameValue *header;  /* HTTP Headers         */   
    _uint8 headerLen;   /* Number of headers    */
    
    nameValue *cookie;  /* Set Cookie           */
    _uint8 cookieLen;   /* Number of cookies    */ 
} orionHttpResponse;


#endif // __ORIONSOCKET_HTTP_H_
