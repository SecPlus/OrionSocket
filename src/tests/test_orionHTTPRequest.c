#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <orion/socket/socket.h>

int main(int argc, char** argv)
{
    httpRequest *req = NULL;
    address victim;
    int i, code;
    
    char *response = NULL;
    const char* domain = "67.205.42.197";
    bzero(victim.domain, DNS_MAXLENGTH);
    strncpy(victim.domain, domain, strlen(domain));
    
    victim.port = 80;
    
    req = (httpRequest *) malloc(sizeof(httpRequest));
    if (req == NULL)
    {
        fprintf(stderr, "[ERROR] Não foi possivel alocar memória.\n");
        return 1;    
    }
    
    orionHTTPRequestInit(req);
    
    req->victim = &victim;
    req->method = METHOD_GET;
    strcpy(req->path, "/");
    
    setHttpHeader(req, "Host", victim.domain);
    setHttpHeader(req, "User-Agent", "Anakin");
                
    code = orionHTTPRequest(req, &response);
    printf("Code: %d\n", code);
    
    if (code == ORIONSOCKET_OK)
    {
        printf("%d\n", i);
        printf("%s\n", response);
        free(response);  
    }  
    
    orionHTTPRequestCleanup(req);
    
    return 0;
}
