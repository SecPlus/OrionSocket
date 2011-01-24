/**
 * Author: Tiago Natel de Moura
 * Date: 14/08/2010
 */
#include <stdio.h>
#include <stdlib.h>
#include <orion/socket/http.h>

int main(int argc, char** argv)
{
	orion_httpRequest *req = NULL;
	char *response = NULL;
	int code;
	
	const char* domain = "universemachine.wordpress.com";
	printf("%s\n", domain);
    // Initialize the library
    orion_initHttpRequest(&req);
    orion_setHttpRequestHost(req, domain, 80);
    orion_setHttpRequestPath(req, "/");
	orion_setHttpRequestHeader(req, "User-Agent", "Anakin SkyWalker");
	
	code = orion_httpRequestPerform(req, &response);

	if (code == ORIONSOCKET_OK)
	{
		printf("%s\n", response);	
		free(response);
	} else {
		printf("code = %d\n", code);
	}
    
    // Free's library
    orion_cleanupHttpRequest(req);
    
	return 0;
}

