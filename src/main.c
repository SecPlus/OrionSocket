#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <sys/socket.h>

int main(int argc, char** argv)
{
    /*address victim;
    strcpy(victim.domain, "www.google.com");
    victim.port = 80;
    char temp[1024];
    int n;
    
    int sockfd = orionTCPConnect(victim);
    
    if (send(sockfd, "GET / HTTP/1.1\n\n", 17, 0) < 0)
    {
        fprintf(stderr, "[ERROR] Não pode enviar dados.\n");
        close(sockfd);
        return 1;
    }
    
    while ((n = read(sockfd, temp, 1023)) > 0)
    {
        printf("%s\n", temp);
        bzero(temp, 1024);
    } */
    
    return 0;
}

