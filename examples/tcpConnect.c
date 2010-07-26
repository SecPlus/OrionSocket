#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include <orion/socket/socket.h>

int main(int argc, char** argv)
{
    
    if (argc < 2)
    {
        fprintf(stderr, "[ERROR] Usage: %s <host> <port>\n", argv[0]);
        return 1;
    }
    
    char temp[1024];
    int n;
    
    int sockfd = orion_tcpConnect(argv[1], atoi(argv[2]));
    
    bzero(temp, 1024);
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
