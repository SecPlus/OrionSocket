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

int orion_tcpConnect(const char* host, _uint16 port)
{
    int sockfd;
    
    struct hostent *addr_host;
    struct sockaddr_in target;
    char* ip = NULL;
    
    if (strlen(host) == 0)
    {
        fprintf(stderr, "Host não pode ser vazio.\n");
        return -1;
    }
    
    if (port <= 0 || port > 65535)
    {
        fprintf(stderr, "Porta inválida.\n");
        return -1;
    } 
    
    
    addr_host = gethostbyname(host);
    if (!addr_host)
    {
        switch (h_errno)
        {
        case HOST_NOT_FOUND:
            fprintf(stderr, "[ERROR] gethostbyname(): Host não encontrado.\n");
            break;
        case NO_ADDRESS:
            fprintf(stderr, "[ERROR] gethostbyname(): O nome solicitado é válido mas não possui um endereço de internet...\n");
            break;
        case TRY_AGAIN:
            fprintf(stderr, "[ERROR] gethostbyname(): Problemas ao obter o endereço do Host!!!\n"
                            "Voce pode tentar novamente...\n"
                            "Esse problema é dificil de identificar a origem...\n"
                            "Não posso lhe ajudar muito...\n"
                            "Esse erro ocorreu na chamada da funcao gethostbyname()\n"
                            "Essa funcao utiliza um banco de dados locais de resolucao de nomes\n"
                            "para encontrar o dominio.\n"
                            "Verifique se possui os arquivos default do resolver, como:\n"
                            "- /etc/hosts\n"
                            "- /etc/networks\n"
                            "- /etc/protocols\n"
                            "- /etc/services\n"
                            "...\n"
                            "Verifique o header '/usr/include/netdb.h'\n");
            break;
        case NO_RECOVERY:
            fprintf(stderr, "[ERROR] gethostbyname(): Erro irrecuperável... Muito explicativo não é? ¬¬\n");
            break;
        default:
            fprintf(stderr, "[ERROR] gethostbyname(): Uwnknow Error Code: %d\n", h_errno);
            break;    
        }
        
        return -1;        
    }
    
    // Converte um endereço de internet (struct in_addr *) em ASCII
    ip = inet_ntoa(*(struct in_addr *)addr_host->h_addr);
    if (*ip == -1)
    {
        fprintf(stderr, "[ERROR] inet_ntoa(): Não foi possivel obter o endereco IP.\n");
        return -1;
    }
        
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
        
        return -1;
    }
    
    target.sin_family = ORION_TCP_FAMILY;
    target.sin_port = htons(port);
    
    // Converte om endereco de internet de ASCII para binário
    if (inet_pton(ORION_TCP_FAMILY, ip, &target.sin_addr) < 0)
    {
        perror("[ERROR] Erro ao setar o endereço remoto.\n");
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
        
        return -1;
    }
    
    return sockfd;
}





