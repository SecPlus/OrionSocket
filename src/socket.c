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

int orion_getHostByName(const char* name, char* buffer)
{
    struct addrinfo hints, *res, *res0;
    struct sockaddr_in * target = NULL;
    int error;
    char *tmp = NULL;
    
    memset(&hints, 0, sizeof(struct addrinfo));
    
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = 0;
    error = getaddrinfo(name, "http", &hints, &res0);
    
    if (error)
    {
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
                freeaddrinfo(res0);
                return ORIONSOCKET_OK;
            }
        }
    }
    
    freeaddrinfo(res0);
    
    return ORIONSOCKET_ADDR_NOTFOUND;
}

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

int orion_tcpConnect(const char* host, _uint16 port)
{
    int sockfd;
    
    struct sockaddr_in target;
    char* ip = NULL;
    
    if (strlen(host) == 0)
    {
        fprintf(stderr, "Host não pode ser vazio.\n");
        return -1;
    }
    
    ip = (char *) malloc(sizeof(char) * (IPv4_MAXLENGTH + 1));
    if (!ip)
        return ORIONSOCKET_ERR_ALLOC;
    
    orion_getHostByName(host, ip);
    
    // Cria o socket
    sockfd = socket(ORION_TCP_FAMILY, ORION_TCP_SOCKETTYPE, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "[ERROR] Erro ao criar socket.\n");
        switch (errno)
        {
        case EAFNOSUPPORT:
            fprintf(stderr, ">> O endereco especificado na 'Familia de Enderecos' não pode"
                            " ser usado com esse socket...\n");
            break;
        case EMFILE:
            fprintf(stderr, ">> A Tabela de descritores por processo está cheia.\n");
            break;
        case ENOBUFS:
            fprintf(stderr, ">> O sistema não possui recursos suficientes para completar a chamada...\n");
            break;
        case ESOCKTNOSUPPORT:
            fprintf(stderr, ">> O socket especificado na familia de enderecos não é siportado.\n");
            break;
        }
        
        free(ip);
        return -1;
    }
    
    bzero(&target, sizeof target);
    target.sin_family = ORION_TCP_FAMILY;
    target.sin_port = htons(port);
    
    // Converte om endereco de internet de ASCII para binário
    if (inet_pton(ORION_TCP_FAMILY, ip, &target.sin_addr) < 0)
    {
        perror("[ERROR] Erro ao setar o endereço remoto.\n");
        ORIONFREE(ip);
        return errno;
    }
    
    memset(target.sin_zero, '\0', 8);
    
    if (connect(sockfd, (struct sockaddr *)&target, sizeof(struct sockaddr)) == -1)
    {
        perror("[ERROR] Erro ao conectar no host.\n");
        
        switch (errno)
        {
        case EADDRINUSE:
            fprintf(stderr, ">> O endereco IP já está em uso...\n");
            break;
        case EADDRNOTAVAIL:
            fprintf(stderr, ">> O endereco especificado não está disponivel nesta maquina local.\n");
            break;
        case EAFNOSUPPORT:
            fprintf(stderr, ">> O endereco na Familia de Enderecos não pode ser usado com esse socket...\n");
            break;
        case EALREADY:
            fprintf(stderr, ">> O socket está setado com O_NONBLOCK  ou O_NDLAY e a conexão anterior ainda não foi completada.\n");
            break;
        case EINTR:
            fprintf(stderr, "The attempt to establish a connection was interrupted by delivery of a signal that was caught; the connection will be established asynchronously.\n");
            break;
        case EACCES:
            fprintf(stderr, "Permissão de busca negada no componente ou acesso de escrita negado no socket.\n");
            break;
        case ENOBUFS:
            fprintf(stderr, "O sistema não possui memória suficiente para a estrutura de dados interna.\n");
            break;
        case EOPNOTSUPP:
            fprintf(stderr, "O socket especificado no parametro 'socket' não suporta a chamada connect.\n");
            break;
        default:
            fprintf(stderr, "Estou com preguiça de escrever um mensagem bonitinha para TODOS os erros que voce comete...\n"
                            "Por favor, crie vergonha na cara e procure pelo codigo de erro %d no arquivo /usr/include/sys/socket.h.\n", errno);
            break;        
        }
        
        ORIONFREE(ip);        
        return -1;
    }
    
    ORIONFREE(ip);
        
    return sockfd;
}

