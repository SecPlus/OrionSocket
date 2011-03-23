/**
 * Experimental source for server side use of OrionSocket
 * ** OrionSocket doesn't works in server side **
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <orion/socket/socket.h>
#include <orion/socket/err.h>

#define BACKLOG 20

_uint8 orion_tcpServer(_uint8* host, _uint16 port, void (* callback)(int));

void handle_client(int newsock)
{
    printf("novo client: %d\n", newsock);
}

int orion_getAddrByName(const char* name, char* buffer)
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


int main(int argc, char** argv)
{
    int sockfd;
    if (argc < 2)
    {
        fprintf(stderr, "[ERROR] Usage: %s <port>\n", argv[0]);
        return 0;
    }
    
    int status = orion_tcpServer("localhost", 8080, handle_client);

    if (ORIONSOCKET_OK != status)
    {
        fprintf(stderr, "Erro ao criar servidor\n");
        return 1; 
    }
    return 0;
}

_uint8 orion_tcpServer(_uint8* host, _uint16 port, void (* callback)(int))
{
    int sockfd, newsock;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    
    sockfd = orion_socket(ORION_TCP_FAMILY, ORION_TCP_SOCKETTYPE, 0);
    
    if (sockfd < 0)
    {
        return sockfd;
    }
    
    serv_addr.sin_family = ORION_TCP_FAMILY;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror(strerror(errno));
        return ORIONSOCKET_BIND_ERROR;
    }
    
    listen(sockfd, BACKLOG);
    
    clilen = sizeof(cli_addr);
    
    while (1)
    {
        newsock = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        
        if (newsock < 0)
        {
            perror(strerror(errno));
            return ORIONSOCKET_ACCEPT_ERROR;
        }
        
        (*callback)(newsock);
    }
    
    return ORIONSOCKET_OK;    
}

