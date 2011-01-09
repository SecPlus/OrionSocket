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
#include "types.h"

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <netdb.h>

#define ORIONSOCKET_OK              0x00

#define ORION_TCP_FAMILY            PF_INET
#define ORION_TCP_SOCKETTYPE        SOCK_STREAM

#define DNS_MAXLENGTH               255
#define URL_MAXLENGTH               2048
#define IPv4_MAXLENGTH              16

#ifdef ORIONSOCKET_DEBUG
#define DEBUG_OUT_HTTPREQUEST(output, req)  do {  \
                                fprintf(output, "================ DEBUGGING HTTP REQUEST ===================\n"); \
                                fprintf(output, "Host: %s\n", req->host); \
                                fprintf(output, "Port: %d\n", req->port); \
                                fprintf(output, "Method: %s\n", orion_getStrMethod(req->method)); \
                                int _orionIntdebug; \
                                for (_orionIntdebug = 0; _orionIntdebug < req->headerLen; _orionIntdebug++) \
                                    fprintf(output, "%s: %s\n", req->header[_orionIntdebug].name, req->header[_orionIntdebug].value); \
                                if (req->cookieLen > 0) \
                                    fprintf(output, "Cookie: "); \
                                for (_orionIntdebug = 0; _orionIntdebug < req->cookieLen; _orionIntdebug++) \
                                    fprintf(output, "%s=%s; &", req->cookie[_orionIntdebug].name, req->cookie[_orionIntdebug].value); \
                                fprintf(output, "===========================================================\n"); \
                                } while(0) 
#define DEBUG_HTTPREQUEST(req)  DEBUG_OUT_HTTPREQUEST(stderr, req)
#define DEBUG_STR(str)			fprintf(stderr, str)
#else
#define DEBUG_HTTPREQUEST(req)
#define DEBUG_OUT_HTTPREQUEST(output, req)
#define DEBUG_OUT_STR(out,str)
#define DEBUG_STR(fmt)
#endif

#define ORIONFREE(ptr)          if (ptr) free(ptr)

/**
 * Retorna o ip do host dado o dominio.
 */
extern int orion_getHostByName(const char* addr, char* buffer);
extern int orion_getDomain(const char* ip, char* buffer);
extern int orion_getDomainByAddr(struct addrinfo* addr, char* buffer);
extern int orion_socket(int domain, int type, int protocol); 
extern int orion_tcpConnect(const char* host, _uint16 port);
#endif // __APISOCKET_

