/*
   OrionSocket - Socket Implementation
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
#include "socket.h"
#include "err.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/* Berkeley Sockets */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "debug.h"

/**
 * Returns ORION_OK (or ZERO) and set in buffer the ip address for the hostname especified by "name". 
 * On error, returns the ERROR CODE returned by getaddrinfo. 
 *
 * @see man getaddrinfo
 * @param name Hostname
 * @param buffer for the ip.
 */
_i16 orion_getHostByName(const char* name, char* buffer)
{
    struct addrinfo hints, *res, *res0 = NULL;
    struct sockaddr_in * target = NULL;
    int error;
    char *tmp = NULL;
    
    memset(&hints, 0, sizeof(struct addrinfo));
    
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = 0;
    error = getaddrinfo(name, "http", &hints, &res0);
    
    if (error)
    {
        if (res0)
            freeaddrinfo(res0);
        return error;
    }
    
    for (res = res0; res; res = res->ai_next)
    {
        target = (struct sockaddr_in *) res->ai_addr;
        if (target)
        {
            tmp = inet_ntoa(target->sin_addr);
            if (tmp && strlen(tmp))
            {
                strncpy(buffer, tmp, strlen(tmp));
                buffer[strlen(tmp)] = '\0';
                if (res0)
                    freeaddrinfo(res0);
                return ORION_OK;
            }
        }
    }
    
    freeaddrinfo(res0);
    
    return ORION_SOCKET_ADDR_NOTFOUND;
}

/**
 * Returns ORION_OK (or ZERO) and set in buffer the hostname of the ip address especified.
 * On error, returns the ERROR CODE of the getaddrinfo().
 * @see getaddrinfo manpage
 * 
 * @param ip 	Ip address
 * @param buffer
 */ 
_i16 orion_getDomain(const char* ip, char* buffer)
{
    struct addrinfo hints, *res0;
    int error;
    
    memset(&hints, 0, sizeof(struct addrinfo));
        
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = 0;
    error = getaddrinfo(ip, NULL, &hints, &res0);
    
    if (error)
    {
        if (res0)
            freeaddrinfo(res0);
        return error;
    }
    
    error = orion_getDomainByAddr(res0, buffer);
    
    freeaddrinfo(res0);
    
    if (error)
        return error;
    
    if (buffer && *buffer && strlen(buffer))
        return ORION_OK;
    else
        return ORION_SOCKET_ERR_UNKNOWN;
}

/**
 * Returns ORION_OK (or ZERO) and set in buffer the hostname for the ip address especified.
 * On error, returns the error code of the getnameinfo() function.
 * @see manpage getnameinfo
 *
 * @param addr	Addrinfo structure with the host data.
 * @param buffer
 */
_i16 orion_getDomainByAddr(struct addrinfo* addr, char* buffer)
{
    struct addrinfo *res;
    int error = ORION_OK;
    
    for (res = addr; res; res = res->ai_next)
    {
        error = getnameinfo(res->ai_addr, res->ai_addrlen, buffer, NI_MAXHOST, NULL, 0, 0);
            
        if (error)
            continue;
                
        if (buffer && *buffer && strlen(buffer))
        {
            error = ORION_OK;
            break;
        }
    }
    
    return error;   
}

/**
 * Connect to host.
 * Returns the socket descriptor for the connection established. On error,
 * -1 is returned and errno is set appropriately.
 */
int orion_tcpConnect(const char* host, _u16 port)
{
    int sockfd;
    
    struct sockaddr_in target;
    char* ip = NULL;
    _i16 error = ORION_OK;
    _u8 count_errors = 0;
    
    if (strlen(host) == 0)
    {
#ifdef ORION_SOCKET_DEBUG
        ORION_DEBUG("<orionsocket> [ERROR] empty host...\n");
#endif
        return ORION_SOCKET_INVALIDHOST;
    }
    
    ip = (char *) malloc(sizeof(char) * (ORION_IPv4_MAXLENGTH + 1));
    if (!ip) 
    {
        return ORION_SOCKET_ERR_ALLOC;
    }
    
    error = orion_getHostByName(host, ip);
#ifdef ORION_SOCKET_DEBUG
   	ORION_DEBUG("%s(%u):%s(%u)\n", host, (unsigned) strlen(host), ip, (unsigned) strlen(ip));
#endif

	if (error) {
		switch (error)
		{
			case ORION_SOCKET_ADDR_NOTFOUND:
#ifdef ORION_SOCKET_DEBUG
				ORION_DEBUG("<orionsocket> [ERROR] Host not found.\n");
#endif
        		ORIONFREE(ip);
        		return ORION_SOCKET_ADDR_NOTFOUND;
        	default:
        		ORIONFREE(ip);
        		return ORION_SOCKET_ERR_UNKNOWN;
		}
	}
	    
    /* Create socket */
    sockfd = orion_socket(ORION_TCP_FAMILY, ORION_TCP_SOCKETTYPE, 0);
    if (sockfd < 0)
    {
        ORIONFREE(ip);
        return sockfd;
    }
    
    memset(&target, '\0', sizeof target);
    target.sin_family = ORION_TCP_FAMILY;
    target.sin_port = htons(port);
    
    /* Convert the address from ASCII to binary */
    if (inet_pton(ORION_TCP_FAMILY, ip, &target.sin_addr) < 0)
    {
#ifdef ORION_SOCKET_DEBUG
        perror("<orionsocket> [ERROR] Error setting remote address.\n");
#endif
        ORIONFREE(ip);
        return errno;
    }
    
    memset(target.sin_zero, '\0', 8);
    
    
    while (connect(sockfd, (struct sockaddr *)&target, sizeof(struct sockaddr)) == -1)
    {
        count_errors++;
#ifdef ORION_SOCKET_DEBUG
        perror("<orionsocket> [ERROR] Erro ao conectar no host.\n");
#endif
        if (count_errors >= ORION_MAX_ERRORCONNECTION)
        {
            ORIONFREE(ip);
            return -1;
        }
    }
    
    ORIONFREE(ip);
        
    return sockfd;
}

int orion_socket(int domain, int type, int protocol)
{
    int sockfd = socket(domain, type, protocol);
    if (sockfd < 0)
    {
#ifdef ORION_SOCKET_DEBUG
        ORION_DEBUG("<orionsocket> [ERROR] Error creating socket.\n");

        switch (errno)
        {
        case EAFNOSUPPORT:
            ORION_DEBUG("<orionsocket> [ERROR] The address specified in the 'address family' can not be used with this socket.\n");
            break;
        case EMFILE:
            ORION_DEBUG("<orionsocket> [ERROR] Table of descriptors per process is full.\n");
            break;
        case ENOBUFS:
            ORION_DEBUG("<orionsocket> [ERROR] The system has insufficient resources to complete the call.\n");
            break;
        case ESOCKTNOSUPPORT:
            ORION_DEBUG("<orionsocket> [ERROR] The socket at the specified address family is not supported.\n");
            break;
        }
#endif        
        return -1;
    }
    
    return sockfd;
}

/**
 * Ensures that all data was sent
 * On success, 1 is returned, otherwise returns 0.
 * 
 * @param int socket - Socket descriptor
 * @param char* buffer- Data to sent
 * @return _u8
 */
_u8 orion_send(int socket, char* buffer)
{
    _u32 bytes_to_send = strlen(buffer);
    _i32 bytes_sent = 0;
    
    while (bytes_to_send > 0)
    {
        bytes_sent = send(socket, buffer, bytes_to_send, 0);
        
        if (bytes_sent < 0)
            return 0;
        bytes_to_send -= bytes_sent;
        buffer += bytes_sent;
    }
    
    return 1;
}

