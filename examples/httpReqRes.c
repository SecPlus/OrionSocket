#include <stdio.h>
#include <stdlib.h>
#include <orion/socket/http.h>

int main(int argc, char** argv)
{
    orion_httpRequest* req = NULL;
    orion_httpResponse* res = NULL;
    int i;
    
    // Inicializa a requisição
    orion_initHttpRequest(&req);
    
    orion_setHttpRequestHost(req, "universemachine.wordpress.com", 80);
    
    // Não há a necessidade de inicializar res (orion_httpResponse)
    int code = orion_httpReqRes(req, &res);
   
    // or if (!code)
    if (code == ORIONSOCKET_OK)  // ORIONSOCKET_OK = 0x00
    {
        /**
         * A resposta encontra-se na estrutura orion_httpResponse *res
         */
        printf("################################\n");
        printf("# HEADERS:\n");
        
        printf("# Http Version: 1.%d\n", res->version);
        printf("# Http Status Code: %d\n", res->code);
        printf("# Server Message: %s\n", res->message);
        printf("# Server Name: %s\n", res->serverName);
        printf("# Server Date: %s\n", res->date);
        printf("# Todos os Headers: \n");
        for (i = 0; i < res->headerLen; i++)
        {
            printf("# %s: %s\n", res->header[i].name, res->header[i].value);
        }

        for (i = 0; i < res->cookieLen; i++)
        {
			printf("# cookie-> %s: %s\n", res->cookie[i].name, res->cookie[i].value);
        }
        
        printf("################################\n");
        printf("# Body Content: \n");
        printf("%s\n", res->body);
    }
    
    // Desaloca a memória da requisição e da resposta        
    orion_cleanupHttpRequest(req);
    orion_cleanupHttpResponse(res);
    
    return 0;
}

