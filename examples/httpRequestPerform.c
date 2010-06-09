#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <orion/socket/socket.h>
#include <orion/socket/http.h>

int main(int argc, char** argv)
{
	orion_httpRequest *req = NULL;
	char *response = NULL;
	int code;
	
	const char* domain = "www.omeuip.com.br";
    // Initialize the library
    orion_httpRequestInit(&req);
    orion_setHttpRequestHost(req, domain, 80);
	orion_setHttpRequestHeader(req, "Host", domain);
	orion_setHttpRequestHeader(req, "User-Agent", "Anakin");
	
	
	code = orion_httpRequestPerform(req, &response);
	
	if (code == ORIONSOCKET_OK)
	{
		printf("%s\n", response);
		
		free(response);
	}
    
    // Free's library
    orion_httpRequestCleanup(req);
    
	return 0;
}

