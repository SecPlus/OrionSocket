#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <orion/socket/http.h>
#include <orion/socket/util.h>

/*void orion_setUrl(orion_httpRequest* req, char* url)*/
/*{*/
/*    char *buffer = NULL, *ptrBuffer = NULL;*/
/*    int it = 0, it2, len;*/
/*    */
/*    len = strlen(url);*/
/*    */
/*    if (!strncmp(url, "https", 5))*/
/*        req->proto = ORION_PROTO_HTTPS;*/
/*    else if (!strncmp(url, "http", 4))*/
/*        req->proto = ORION_PROTO_HTTP;*/
/*        */
/*    printf("PROTO=%d\n", req->proto);*/
/*    */
/*    //for (it = 0; it < len && url[it] != ':'; it++);*/
/*    it = strcspn(url, ":");*/

/*    if (it == len)*/
/*        return;*/
/*    */
/*    */
/*    buffer = strdup(url + it + 3);  // pula '://'*/
/*    ptrBuffer = buffer;*/
/*    len = strlen(buffer);*/
/*    */
/*    // verifica se tem autenticação*/
/*    it = strcspn(buffer, "@");*/
/*    if (it < len)*/
/*    {*/
/*        ptrBuffer[it] = '\0';*/
/*        // verifica se tem senha*/
/*        it2 = strcspn(buffer, ":");*/
/*        if (it < len)*/
/*        {*/
/*            ptrBuffer[it2] = '\0';*/
/*            req->auth = strdup(buffer);*/
/*            printf("login: %s\n", ptrBuffer);*/
/*            ptrBuffer = ptrBuffer + it2 + 1;*/
/*            req->pass = strdup(buffer);*/
/*            printf("senha: %s\n", ptrBuffer);*/
/*            ptrBuffer += it - it2;*/
/*        } else {*/
/*            req->auth = strdup(buffer);*/
/*            req->pass = NULL;*/
/*            ptrBuffer += it + 1;*/
/*            printf("%s\n", ptrBuffer);*/
/*        }    */
/*    }*/
/*    */
/*    len = strlen(ptrBuffer);*/
/*    int matchLen = 0;*/
/*    it = strcspn(ptrBuffer, ":/");*/
/*    it2 = 0;*/
/*    */
/*    if (it > 0)*/
/*    {*/
/*        if (it == len)*/
/*        {*/
/*            req->host = strdup(ptrBuffer);*/
/*            req->port = 80;*/
/*            ptrBuffer += it;*/
/*            matchLen += it;*/
/*        } else*/
/*        */
/*        // verifica se a porta não é a default*/
/*        if (ptrBuffer[it] == ':')*/
/*        {*/
/*            ptrBuffer[it] = '\0';*/
/*            req->host = strdup(ptrBuffer);*/
/*            printf("host: %s\n", ptrBuffer);*/
/*            ptrBuffer += it + 1;*/
/*            matchLen += it + 1;*/
/*            it2 = strcspn(ptrBuffer, "/");*/
/*            ptrBuffer[it2] = '\0';*/
/*            req->port = atoi(ptrBuffer);*/
/*            printf("port: %s\n", ptrBuffer);*/
/*            */
/*            ptrBuffer += it2 + 1;*/
/*            matchLen += it2;*/
/*        } else {*/
/*            ptrBuffer[it] = '\0';*/
/*            req->host = strdup(ptrBuffer);*/
/*            printf("host: %s\n", ptrBuffer);*/
/*            ptrBuffer += it + 1;*/
/*            matchLen += it;*/
/*        }*/
/*    } else*/
/*        return;*/

/*    if (matchLen >= len)*/
/*    {*/
/*        req->path = strdup("/");*/
/*        return;*/
/*    }*/
/*    */
/*    printf("path: /%s\n", ptrBuffer);*/
/*    req->path = strdup(ptrBuffer);*/
/*        */
/*    free(buffer);*/
/*}*/

int main(int argc, char** argv)
{
    orion_httpRequest* req = NULL;    
    orion_initHttpRequest(&req);
    orion_setUrl(req, "https://i4k:1123581321@www.google.com:8080/path/to/directory?param1=teste1&param2=teste2");
    ORION_DEBUG_HTTPREQUEST(req);
    orion_cleanupHttpRequest(req);
    
    puts("");
    orion_initHttpRequest(&req);
    orion_setUrl(req, "https://i4k@www.google.com/path/to/directory?param1=teste1&param2=teste2");
    ORION_DEBUG_HTTPREQUEST(req);
    orion_cleanupHttpRequest(req);
    
    puts("");
    orion_initHttpRequest(&req);
    orion_setUrl(req, "http://www.google.com/path/to/directory?param1=teste1&param2=teste2");
    ORION_DEBUG_HTTPREQUEST(req);
    orion_cleanupHttpRequest(req);
    
    puts("");
    orion_initHttpRequest(&req);
    orion_setUrl(req, "http://www.google.com/");
    ORION_DEBUG_HTTPREQUEST(req);
    orion_cleanupHttpRequest(req);
    
    puts("");
    orion_initHttpRequest(&req);
    orion_setUrl(req, "http://www.google.com");
    ORION_DEBUG_HTTPREQUEST(req);
    orion_cleanupHttpRequest(req);
    return 0;
}

