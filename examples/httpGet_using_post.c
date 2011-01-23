#include <stdio.h>
#include <orion/socket/http.h>

void handleResponse(char* buffer, _uint32 len);

int main(int argc, char** argv)
{
    orion_httpRequest* request = NULL;
    orion_initHttpRequest(&request);
    orion_setHttpRequestHost(request, "registro.br", 80);
    orion_setHttpRequestPath(request, "/cgi-bin/whois/");
    orion_setHttpRequestMethod(request, "POST");
    orion_setHttpRequestQuery(request, "qr=www.bugsec.com.br");
    orion_setHttpRequestOption(request, ORION_OPTDEBUG_REQUEST);
    orion_setHttpRequestHeader(request, "User-Agent", "Mozilla/1.0");
    
    orion_httpGet(request, handleResponse, 1024);
    
    orion_cleanupHttpRequest(request);
    
    return 0;
}

void handleResponse(char* buffer, _uint32 len)
{
    printf("%s", buffer);
}


