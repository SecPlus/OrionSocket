#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <orion/socket/socket.h>

int main(int argc, char** argv)
{
    orionHttpRequest *req = NULL;
    int code;
    
    char *response = NULL;
    const char* domain = "www.omeuip.com.br";
    
    req = (orionHttpRequest *) malloc(sizeof(orionHttpRequest));
    if (req == NULL)
    {
        fprintf(stderr, "[ERROR] Não foi possivel alocar memória.\n");
        return 1;    
    }
    
    orionHttpRequestInit(req);
    
    req->host = strdup(domain);
    req->method = METHOD_GET;
    req->path = strdup("/");
        
    orionSetHttpRequestHeader(req, "Host", domain);
    orionSetHttpRequestHeader(req, "User-Agent", "Anakin");
                
    code = orionHttpRequestPerform(req, &response);
    printf("Code: %d\n", code);
    
    if (code == ORIONSOCKET_OK)
    {
        printf("%d\n", code);
        printf("%s\n", response);
        free(response);  
    }  
    
    orionHttpRequestCleanup(req);
    
    return 0;
}
