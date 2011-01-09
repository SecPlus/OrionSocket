#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <orion/socket/http.h>

int main()
{
	orion_httpRequest* req = NULL;
	orion_httpResponse* resp = null;

	orion_initHttpRequest(&req);
	orion_initHttpResponse(&res);
	orion_setHttpRequestHost(req, "www.google.com");
	
	
	return 0;
}


