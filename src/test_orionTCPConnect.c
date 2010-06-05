#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "socket.h"

int main(int argc, char** argv)
{
    address victim;
    
    if (argc < 2)
    {
        fprintf(stderr, "[ERROR] Usage: %s <host> <port>\n", argv[0]);
        return 1;
    }
    strcpy(victim.domain, argv[1]);
    victim.port = atoi(argv[2]);
    char temp[1024];
    int n;
    
    int sockfd = orionTCPConnect(victim);
    
    if (send(sockfd, "GET / HTTP/1.1\nHost: xxx\n\n", 26, 0) < 0)
    {
        fprintf(stderr, "[ERROR] Não pode enviar dados.\n");
        close(sockfd);
        return 1;
    }
    
    while ((n = read(sockfd, temp, 1023)) > 0)
    {
        printf("%s\n", temp);
        bzero(temp, 1024);
    }
    
    close(sockfd);
    
    return 0;
}
