/*
   OrionSocket - Socket Header
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
#ifndef __ORIONSOCKET_
#define __ORIONSOCKET_

#include <stdint.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "api.h"
#include "http.h"

#define ORIONSOCKET_OK              0x00

#define METHOD_GET                  0x01
#define METHOD_POST                 0x02
#define METHOD_TRACE                0x03
#define METHOD_PUT                  0x04
#define METHOD_DELETE               0x05

#define ORION_TCP_FAMILY            AF_INET
#define ORION_TCP_SOCKETTYPE       SOCK_STREAM

#define HTTP_PROTOCOL               "HTTP/1.1"
#define HTTP_REQUEST_MAXLENGTH      2048
#define HTTP_RESPONSE_LENGTH        1024
#define HTTP_BIG_RESPONSE           8192

#define DNS_MAXLENGTH               255
#define URL_MAXLENGTH               2048
#define IP_MAXLENGTH                16

#ifdef ORIONSOCKET_DEBUG
#define DEBUG_HTTPREQUEST(req)  do {  \
                                printf("DEBUGGING HTTP REQUEST:\n"); \
                                printf("Host: %s\n", req->host); \
                                printf("Port: %d\n", req->port); \
                                printf("Method: %s\n", _getStrMethod(req->method)); \
                                int _orionIntdebug; \
                                for (_orionIntdebug = 0; _orionIntdebug < req->headerLen; _orionIntdebug++) \
                                    printf("%s: %s\n", req->header[_orionIntdebug].name, req->header[_orionIntdebug].value); \
                                if (req->cookieLen > 0) \
                                    printf("Cookie: "); \
                                for (_orionIntdebug = 0; _orionIntdebug < req->cookieLen; _orionIntdebug++) \
                                    printf("%s=%s; &", req->cookie[_orionIntdebug].name, req->cookie[_orionIntdebug].value); \
                                } while(0) 
#else
#define DEBUG_HTTPREQUEST(req)
#endif

extern void orion_httpRequestInit(orion_httpRequest *req);
extern void orion_httpRequestCleanup(orion_httpRequest *req);
extern _uint8 orion_setHttpRequestHeader(orion_httpRequest *req, const char* name, const char* value);
extern void orion_setHttpRequestHost(orion_httpRequest *req, const char* name);
extern void orion_assemblyHttpRequest(orion_httpRequest *req, char* reqBuffer);
extern int orion_tcpConnect(const char* host, _uint16 port);
extern _uint8 orion_httpRequestPerform(orion_httpRequest *req, char** response);

#endif // __APISOCKET_
