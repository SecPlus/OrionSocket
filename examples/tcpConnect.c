#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include <orion/socket/socket.h>

#define LEN 1024

int main(int argc, char** argv)
{
    
    if (argc < 2)
    {
        fprintf(stderr, "[ERROR] Usage: %s <host> <port>\n", argv[0]);
        return 0;
    }
    
    char temp[LEN];
    int n;
    
    int sockfd = orion_tcpConnect(argv[1], atoi(argv[2]));
    
    bzero(temp, LEN);
    if (send(sockfd, "GET / HTTP/1.1\nHost: xxx\n\n", 26, 0) < 0)
    {
        fprintf(stderr, "[ERROR] Não pode enviar dados.\n");
        close(sockfd);
        return 1;
    }
    
    while ((n = read(sockfd, temp, LEN-1)) > 0)
    {
        printf("%s\n", temp);
        bzero(temp, LEN);
    }
    
    close(sockfd);
    
    return 0;
}
