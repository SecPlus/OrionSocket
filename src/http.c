/*
   OrionSocket - Socket Header
   --------------------------------

   Author: Tiago Natel de Moura <tiago4orion@gmail.com>

   Copyright 2007, 2008 by Tiago Natel de Moura. All Rights Reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 */
#include "http.h"
#include "err.h"
#include "socket.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

// Berkeley Sockets
#include <netdb.h>
#include <sys/socket.h>

void orion_initHttpRequest(orion_httpRequest **req2)
{
  orion_httpRequest *req = NULL;
    
  req = (orion_httpRequest *) malloc(sizeof(orion_httpRequest));
  if (!req)
  {
    fprintf(stderr, "[ORION][ERROR] Não foi possivel alocar memória.\n"); 
    return;
  }

  req->host = NULL;
  req->port = 80;                 /* Default port         */
  req->path = NULL;
  req->method = METHOD_GET;       /* Default HTTP Method  */
  req->file_ext = NULL;
  req->header = NULL;
  req->headerLen = 0;
  req->cookie = NULL;
  req->cookieLen = 0;
  req->query = NULL;
  req->option = 0x0;             /* No option */
    
  *req2 = req;
}

void orion_cleanupHttpRequest(orion_httpRequest *req)
{
    int i;
    
    if (req)
    {
        ORIONFREE(req->host);
        req->port = 80;
        ORIONFREE(req->path);
        ORIONFREE(req->file_ext);

        for (i = 0; i < req->headerLen; i++)
        {
            ORIONFREE(req->header[i].name);
            ORIONFREE(req->header[i].value);
        }

        if (req->headerLen > 0)
            ORIONFREE(req->header);
        req->headerLen = 0;

        for (i = 0; i < req->cookieLen; i++)
        {
            ORIONFREE(req->cookie[i].name);
            ORIONFREE(req->cookie[i].value);
        }

        if (req->cookieLen > 0)
            ORIONFREE(req->cookie);
        req->cookieLen = 0;    

        ORIONFREE(req);
    }
}

void orion_setHttpRequestHost(orion_httpRequest *req, const char* host, _uint16 port)
{
    req->host = strdup(host);
    req->port = port;
}

void orion_setHttpRequestPath(orion_httpRequest *req, const char* path)
{
    ORIONFREE(req->path);
    req->path = strdup(path);
}

//
// Adiciona um Header à requisição HTTP da estrutura httpRequest
// @param orionHttpRequest*   req
// @param const char* name
// @param const char* value
_uint8 orion_setHttpRequestHeader(orion_httpRequest *req, const char* name, const char* value)
{
	_uint8 len = 0, lenName = 0, lenValue = 0;
	len = req->headerLen;
	
    lenName = strlen(name);
    lenValue = strlen(value);
    
	req->header = (nameValue *) orion_realloc(req->header, sizeof(nameValue)*(len+1));
	if (!req->header)
	{
		fprintf(stderr, "[ERROR] Erro ao alocar memória.\n");
		return ORIONSOCKET_ERR_ALLOC;
	}
    
	req->header[len].name = (char *) malloc(sizeof(char) * lenName + 1);
	req->header[len].value = (char *) malloc(sizeof(char) * lenValue + 1);
	if (!req->header[len].name || !req->header[len].value)
	{
		fprintf(stderr, "[ERROR] Erro ao alocar memória.\n");
		return ORIONSOCKET_ERR_ALLOC;
	}
	
	strncpy(req->header[len].name, name, lenName);
	strncpy(req->header[len].value, value, lenValue);
	req->header[len].name[lenName] = '\0';
	req->header[len].value[lenValue] = '\0';
	
    
	req->headerLen++;
    
	return ORIONSOCKET_OK;    
}

void orion_setHttpRequestOption(orion_httpRequest* req, _uint16 option)
{
    req->option |= option;
}

// Estabelece uma conexão única no host passado em httpRequest *
// Retorna o conteudo da página.
// 
_uint8 orion_httpRequestPerform(orion_httpRequest *req, char** response)
{
	int sockfd, n = 0, lengthBuffer = 0;
	char reqBuffer[HTTP_REQUEST_MAXLENGTH], temp[HTTP_BIG_RESPONSE];
	char *localBuffer = NULL;
    
	memset(reqBuffer, '\0', sizeof(char) * HTTP_REQUEST_MAXLENGTH);
	memset(temp, '\0', sizeof(char) * HTTP_RESPONSE_LENGTH);
    
	orion_assemblyHttpRequest(req, reqBuffer);
    
	DEBUG_HTTPREQUEST(req);

    if ((req->option & ORION_OPTDEBUG_REQUEST)==ORION_OPTDEBUG_REQUEST)
        printf("%s\n", reqBuffer);

    //printf("option=0x%08x\n", req->option);
    
	sockfd = orion_tcpConnect(req->host, req->port);
    
	if (sockfd < 0)
	{
		return errno;
	}
    
	if (send(sockfd, reqBuffer, strlen(reqBuffer), 0) < 0)
	{
		perror("[ERROR] Erro ao enviar requisição.\n");
		close(sockfd);
		return errno;
	}
    
	localBuffer = (char *) malloc(sizeof(char) * 1);
	localBuffer[0] = '\0';
    
	while((n = read(sockfd, temp, HTTP_BIG_RESPONSE-1)) > 0)
	{
        lengthBuffer = n + strlen(localBuffer) + 1;
		localBuffer = (char *) orion_realloc(localBuffer, lengthBuffer);
		temp[n-1] = '\0';
		strncat(localBuffer, temp, lengthBuffer-1);
		bzero(temp, HTTP_BIG_RESPONSE);       
	}
    
	close(sockfd);
    
	*response = localBuffer;
    
	return ORIONSOCKET_OK;
}

_uint8 orion_httpGet(orion_httpRequest* req, void (* callback)(char*,_uint32), _uint32 count)
{
    int sockfd, n;
    char reqBuffer[HTTP_REQUEST_MAXLENGTH], responseBuffer[count];
    
    memset(reqBuffer, 0, HTTP_REQUEST_MAXLENGTH);
    memset(responseBuffer, 0, count);
    
    orion_assemblyHttpRequest(req, reqBuffer);
    
    DEBUG_HTTPREQUEST(req);

    if ((req->option & ORION_OPTDEBUG_REQUEST)==ORION_OPTDEBUG_REQUEST)
        printf("%s\n", reqBuffer);

    sockfd = orion_tcpConnect(req->host, req->port);
    if (sockfd < 0)
        return errno;
    
    if (write(sockfd, reqBuffer, strlen(reqBuffer)) < 0)
    {
		perror("[ERROR] Erro ao enviar requisição.\n");
		close(sockfd);
		return errno;
	}
	
	n = read(sockfd, responseBuffer, count-1);
		
	while (n > 0)
	{
	    responseBuffer[n] = '\0';
	    (*callback)(responseBuffer, n);
	    memset(responseBuffer, 0, count);
	    n = read(sockfd, responseBuffer, count-1);
	}
	
	close(sockfd);
	
	return ORIONSOCKET_OK;
}

// Monta a Requisição HTTP a partir da estrutura httpRequest
// @param orionHttpRequest*      req
// @param char *            reqBuffer
void orion_assemblyHttpRequest(orion_httpRequest* req, char* reqBuffer)
{
	_uint32 size = 0, i;
	char temp[10];
	bzero(temp, 10);
    
    // It is safe.
	strcpy(reqBuffer, orion_getStrMethod(req->method));
	strncat(reqBuffer, " ", HTTP_REQUEST_MAXLENGTH-1);
	if (!req->path)
	    req->path = strdup("/");
	
	strncat(reqBuffer, req->path, HTTP_REQUEST_MAXLENGTH-1);
	strncat(reqBuffer, " ", HTTP_REQUEST_MAXLENGTH-1);
	strncat(reqBuffer, HTTP_PROTOCOL, HTTP_REQUEST_MAXLENGTH-1);
	strncat(reqBuffer, "\n", HTTP_REQUEST_MAXLENGTH-1);    
    
    _uint8 _host_header_exists = 0;

    for (i = 0; i < req->headerLen; i++)
        if (strcasecmp(req->header[i].name, "Host") == 0)
            _host_header_exists = 1;

    if (_host_header_exists == 0)
    {
        strncat(reqBuffer, "Host: ", HTTP_REQUEST_MAXLENGTH-1);
        strncat(reqBuffer, req->host, HTTP_REQUEST_MAXLENGTH-1);
        strncat(reqBuffer, "\n", HTTP_REQUEST_MAXLENGTH);        
    }
    
	for (i = 0; i < req->headerLen; i++)
	{
		strncat(reqBuffer, req->header[i].name, HTTP_REQUEST_MAXLENGTH-1);
		strncat(reqBuffer, ": ", HTTP_REQUEST_MAXLENGTH-1);
		strncat(reqBuffer, req->header[i].value, HTTP_REQUEST_MAXLENGTH-1);
		strncat(reqBuffer, "\n", HTTP_REQUEST_MAXLENGTH-1);
	}

	if (req->method == METHOD_POST)
	{
		strncat(reqBuffer, "Content-Length: ", HTTP_REQUEST_MAXLENGTH-1);

		size += strlen(req->query);     

		sprintf(temp, "%d\n", size);
		strncat(reqBuffer, temp, HTTP_REQUEST_MAXLENGTH-1);

		strncat(reqBuffer, req->query, HTTP_REQUEST_MAXLENGTH-1);
	}
 
	strncat(reqBuffer, "\n", HTTP_REQUEST_MAXLENGTH-1);
}

