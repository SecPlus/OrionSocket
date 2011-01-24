/*
   OrionSocket - Socket Implementation
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
#include "socket.h"
#include "err.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Berkeley Sockets
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

static int orionsocket_last_error = ORIONSOCKET_OK;

/**
 *	TODO: Change #ifdef ORIONSOCKET_DEBUG for a include library with MACRO's defined when DEBUG is enabled.
 */
/**
 * Returns ORIONSOCKET_OK (or ZERO) and set in buffer the ip address for the hostname especified by "name". 
 * On error, returns the ERROR CODE returned by getaddrinfo. 
 *
 * @see man getaddrinfo
 * @param name Hostname
 * @param buffer for the ip.
 */
int orion_getHostByName(const char* name, char* buffer)
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
                return ORIONSOCKET_OK;
            }
        }
    }
    
    freeaddrinfo(res0);
    
    return ORIONSOCKET_ADDR_NOTFOUND;
}

/**
 * Returns ORIONSOCKET_OK (or ZERO) and set in buffer the hostname of the ip address especified.
 * On error, returns the ERROR CODE of the getaddrinfo().
 * @see getaddrinfo manpage
 * 
 * @param ip 	Ip address
 * @param buffer
 */ 
int orion_getDomain(const char* ip, char* buffer)
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
        return ORIONSOCKET_OK;
    else
        return ORIONSOCKET_ERR_UNKNOWN;
}

/**
 * Returns ORIONSOCKET_OK (or ZERO) and set in buffer the hostname for the ip address especified.
 * On error, returns the error code of the getnameinfo() function.
 * @see manpage getnameinfo
 *
 * @param addr	Addrinfo structure with the host data.
 * @param buffer
 */
int orion_getDomainByAddr(struct addrinfo* addr, char* buffer)
{
    struct addrinfo *res;
    int error = ORIONSOCKET_OK;
    
    for (res = addr; res; res = res->ai_next)
    {
        error = getnameinfo(res->ai_addr, res->ai_addrlen, buffer, NI_MAXHOST, NULL, 0, 0);
            
        if (error)
            continue;
                
        if (buffer && *buffer && strlen(buffer))
        {
            error = ORIONSOCKET_OK;
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
int orion_tcpConnect(const char* host, _uint16 port)
{
    int sockfd;
    
    struct sockaddr_in target;
    char* ip = NULL;
    _uint8 error = ORIONSOCKET_OK;
    
    if (strlen(host) == 0)
    {
#ifdef ORIONSOCKET_DEBUG
        fprintf(stderr, "<orionsocket> [ERROR] empty host...\n");
#endif
		orionsocket_last_error = ORIONSOCKET_INVALIDHOST;
        return -1;
    }
    
    ip = (char *) malloc(sizeof(char) * (IPv4_MAXLENGTH + 1));
    if (!ip) 
    {
    	orionsocket_last_error = ORIONSOCKET_ERR_ALLOC;
        return -1;
    }
    
    error = orion_getHostByName(host, ip);
#ifdef ORIONSOCKET_DEBUG
   	fprintf(stderr, "%s(%u):%s(%u)\n", host, (unsigned) strlen(host), ip, (unsigned) strlen(ip));
#endif

	if (error) {
		switch (error)
		{
			case ORIONSOCKET_ADDR_NOTFOUND:
#ifdef ORIONSOCKET_DEBUG
				fprintf(stderr, "<orionsocket> [ERROR] Host not found.\n");
#endif
        		ORIONFREE(ip);
        		orionsocket_last_error = ORIONSOCKET_ADDR_NOTFOUND;
        		return -1;
        	default:
        		orionsocket_last_error = ORIONSOCKET_ERR_UNKNOWN;
        		ORIONFREE(ip);
        		return -1;        		
		}
	}
	    
    // Cria o socket
    sockfd = orion_socket(ORION_TCP_FAMILY, ORION_TCP_SOCKETTYPE, 0);
    if (sockfd < 0)
    {
        ORIONFREE(ip);
        return sockfd;
    }
    
    bzero(&target, sizeof target);
    target.sin_family = ORION_TCP_FAMILY;
    target.sin_port = htons(port);
    
    // Converte um endereco de internet de ASCII para binário
    if (inet_pton(ORION_TCP_FAMILY, ip, &target.sin_addr) < 0)
    {
#ifdef ORIONSOCKET_DEBUG
        perror("<orionsocket> [ERROR] Erro ao setar o endereço remoto.\n");
#endif
        ORIONFREE(ip);
        return errno;
    }
    
    memset(target.sin_zero, '\0', 8);
    
    _uint8 count_errors = 0;
    
    while (connect(sockfd, (struct sockaddr *)&target, sizeof(struct sockaddr)) == -1)
    {
        count_errors++;
#ifdef ORIONSOCKET_DEBUG
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
#ifdef ORIONSOCKET_DEBUG
        fprintf(stderr, "<orionsocket> [ERROR] Error creating socket.\n");

        switch (errno)
        {
        case EAFNOSUPPORT:
            fprintf(stderr, "<orionsocket> [ERROR] The address specified in the 'address family' can not be used with this socket.\n");
            break;
        case EMFILE:
            fprintf(stderr, "<orionsocket> [ERROR] Table of descriptors per process is full.\n");
            break;
        case ENOBUFS:
            fprintf(stderr, "<orionsocket> [ERROR] The system has insufficient resources to complete the call.\n");
            break;
        case ESOCKTNOSUPPORT:
            fprintf(stderr, "<orionsocket> [ERROR] The socket at the specified address family is not supported.\n");
            break;
        }
#endif        
        return -1;
    }
    
    return sockfd;
}
