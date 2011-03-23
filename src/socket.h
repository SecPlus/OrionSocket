/*
   OrionSocket - Socket Header
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
#ifndef __ORIONSOCKET_
#define __ORIONSOCKET_

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "types.h"
#include "err.h"

#if HAVE_CONFIG_H
#include <config.h>
#endif


#define ORION_OK              0x00

#define ORION_TCP_FAMILY            PF_INET
#define ORION_TCP_SOCKETTYPE        SOCK_STREAM

#define ORION_DNS_MAXLENGTH               255
#define ORION_URL_MAXLENGTH               2048
#define ORION_IPv4_MAXLENGTH              16
#define ORION_MAX_ERRORCONNECTION   3

#ifdef ORIONSOCKET_DEBUG
#define ORION_DEBUG_STR(str)			fprintf(stderr, str)
#else
#define ORION_DEBUG_OUT_STR(out,str)
#define ORION_DEBUG_STR(fmt)
#endif

#define ORIONFREE(ptr)          if (ptr) free(ptr)

/**
 * Retorna o ip do host dado o dominio.
 */
extern _i16 orion_getHostByName(const char* addr, char* buffer);
extern _i16 orion_getDomain(const char* ip, char* buffer);
extern _i16 orion_getDomainByAddr(struct addrinfo* addr, char* buffer);
extern int orion_socket(int domain, int type, int protocol); 
extern int orion_tcpConnect(const char* host, _u16 port);
extern _u8 orion_send(int socket, char* buffer);
#endif /* __ORIONSOCKET_ */

