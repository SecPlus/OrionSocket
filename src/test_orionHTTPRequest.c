#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket.h"

int main(int argc, char** argv)
{
    httpRequest *req = NULL;
    address victim;
    int i;
    
    char *response = NULL;
    const char* domain = "www.omeuip.com.br";
    bzero(victim.domain, DNS_MAXLENGTH);
    strncpy(victim.domain, domain, strlen(domain));
    
    victim.port = 80;
    
    req = (httpRequest *) malloc(sizeof(httpRequest));
    if (req == NULL)
    {
        fprintf(stderr, "[ERROR] Não foi possivel alocar memória.\n");
        return 1;    
    }
    
    req->victim = &victim;
    req->method = METHOD_GET;
    req->path = "/";
    req->header = NULL;
    req->headerLen = 0;
    req->paramLen = 0;

    setHttpHeader(req, "Host", victim.domain);
    setHttpHeader(req, "User-Agent", "Anakin");
                
    i = orionHTTPRequest(req, &response);
    
    printf("%d\n", i);
    printf("%s\n", response);
    
    free(response);
    
    httpRequestCleanup(req);
    
    return 0;
}
