#include <stdio.h>
#include <orion/socket/http.h>

void myCallBack(char* buffer, unsigned int count)
{
    printf("%s", buffer);
}

int main(int argc, char** argv)
{
    orion_httpRequest* req = NULL;
    
    orion_initHttpRequest(&req);
    orion_setHttpRequestHost(req, "universemachine.wordpress.com", 80);
    
    int code = orion_httpGet(req, myCallBack, 100);
    
    if (code == ORIONSOCKET_OK)
        printf("\n[+] requisição realizada com sucesso.\n");
    else
        printf("\n[-] erro ao fazer requisição...\ncodigo de erro=%d\n", code);        
    
    orion_cleanupHttpRequest(req);
    return 0;
}
