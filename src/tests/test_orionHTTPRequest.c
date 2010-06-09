#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <orion/socket/socket.h>
#include <orion/socket/http.h>

int main(int argc, char** argv)
{
    orion_httpRequest *req = NULL;
    int code;
    
    char *response = NULL;
    const char* domain = "www.omeuip.com.br";
    
    orion_httpRequestInit(&req);
    
    req->host = strdup(domain);
    req->method = METHOD_GET;
    req->path = strdup("/");
        
    orion_setHttpRequestHeader(req, "Host", domain);
    orion_setHttpRequestHeader(req, "User-Agent", "Anakin");
                
    code = orion_httpRequestPerform(req, &response);

    if (code == ORIONSOCKET_OK)
    {
        printf("%d\n", code);
        printf("%s\n", response);
        free(response);  
    }  
    
    orion_httpRequestCleanup(req);
    
    return 0;
}
