/** 
 *******************************************************************************
 *  _______       _____             ________           ______      _____       *
 *  __  __ \_________(_)______________  ___/______________  /________  /_      *
 *  _  / / /_  ___/_  /_  __ \_  __ \____ \_  __ \  ___/_  //_/  _ \  __/      *
 *  / /_/ /_  /   _  / / /_/ /  / / /___/ // /_/ / /__ _  ,<  /  __/ /_        *
 *  \____/ /_/    /_/  \____//_/ /_//____/ \____/\___/ /_/|_| \___/\__/        *
 *                                                                             *
 *******************************************************************************
 *******************************************************************************
 * This example shows how to get a remote web page in few steps.               *
 *******************************************************************************
 *******************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <orion/socket/http.h>

int main(int argc, char** argv, char** envp)
{
    orion_httpRequest* req = NULL;
    char* response = NULL;
    
    orion_initHttpRequest(&req);
    orion_setHttpRequestHost(req, "example.com", 80);
    orion_setHttpRequestPath(req, "/");
    
    _uint8 code = orion_httpRequestPerform(req, &response);
    
    if (!code)
    {
        printf("%s\n", response);
        free(response);
    }
    
    orion_cleanupHttpRequest(req);
    return 0;
}

