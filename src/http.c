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

void orion_httpRequestInit(orion_httpRequest **req2)
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
  req->options = 0x0;             /* No option */
    
  *req2 = req;
}

void orion_httpRequestCleanup(orion_httpRequest *req)
{
  int i;
  if (req->host)
    free(req->host);
  req->port = 80;
  if (req->path)
    free(req->path);
  if (req->file_ext)
    free(req->file_ext);
    
  for (i = 0; i < req->headerLen; i++)
  {
    free(req->header[i].name);
    free(req->header[i].value);
  }
    
  if (req->headerLen > 0)
    free(req->header);
  req->headerLen = 0;
    
  for (i = 0; i < req->cookieLen; i++)
  {
    free(req->cookie[i].name);
    free(req->cookie[i].value);
  }
    
  if (req->cookieLen > 0)
    free(req->cookie);
  req->cookieLen = 0;    
    
  if (req)
    free(req);
}

void orion_setHttpRequestHost(orion_httpRequest *req, const char* host, _uint16 port)
{
	req->host = strdup(host);
	req->port = port;
}

void orion_setHttpRequestPath(orion_httpRequest *req, const char* path)
{
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
	char* tmp = NULL;
	len = req->headerLen;
	int error;
    
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
	
	if (!strcmp(name, "Host") || !strcmp(name, "host"))
	{
	    tmp = (char *) malloc(sizeof(char) * NI_MAXHOST);
	    
	    if (!tmp)
	    {
	        ORIONFREE(req->header[len].name);
	        ORIONFREE(req->header[len].value);
	        return ORIONSOCKET_ERR_ALLOC;
	    }
        
        bzero(tmp, NI_MAXHOST);
        if ((error = orion_getDomain(value, tmp)) != ORIONSOCKET_OK)
        {
            printf("[ERROR %d] : %s\n", error, gai_strerror(error));
        }
        
	    strncpy(req->header[len].name, "Host", lenName);
	    strncpy(req->header[len].value, tmp, lenValue);
	    req->header[len].name[lenName] = '\0';
	    req->header[len].value[lenValue] = '\0';
	    
	    free(tmp);
	} else {
	    strcpy(req->header[len].name, name);
	    strcpy(req->header[len].value, value);
	}
    
	req->headerLen++;
    
	return ORIONSOCKET_OK;    
}

// Estabelece uma conexão única no host passado em httpRequest *
// Retorna o conteudo da página.
// 
_uint8 orion_httpRequestPerform(orion_httpRequest *req, char** response)
{
	int sockfd, n = 0;
	char reqBuffer[HTTP_REQUEST_MAXLENGTH], temp[HTTP_BIG_RESPONSE];
	char *localBuffer = NULL;
    
	memset(reqBuffer, '\0', sizeof(char) * HTTP_REQUEST_MAXLENGTH);
	memset(temp, '\0', sizeof(char) * HTTP_RESPONSE_LENGTH);
    
	orion_assemblyHttpRequest(req, reqBuffer);
    
	DEBUG_HTTPREQUEST(req);
    
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
		localBuffer = (char *) orion_realloc(localBuffer, n + strlen(localBuffer) + 1);
		temp[n-1] = '\0';
		strcat(localBuffer, temp);
		bzero(temp, HTTP_BIG_RESPONSE);       
	}
    
	close(sockfd);
    
	*response = localBuffer;
    
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
    
	strcpy(reqBuffer, orion_getStrMethod(req->method));
	strcat(reqBuffer, " ");
	if (!req->path)
	    req->path = strdup("/");
	
	strcat(reqBuffer, req->path);
	strcat(reqBuffer, " ");
	strcat(reqBuffer, HTTP_PROTOCOL);
	strcat(reqBuffer, "\n");    
    
	for (i = 0; i < req->headerLen; i++)
	{
		strcat(reqBuffer, req->header[i].name);
		strcat(reqBuffer, ": ");
		strcat(reqBuffer, req->header[i].value);
		strcat(reqBuffer, "\n");
	}
    
	if (req->method == METHOD_POST)
	{
		strcat(reqBuffer, "Content-Length: ");

		size += strlen(req->query);     

		sprintf(temp, "%d\n", size);
		strcat(reqBuffer, temp);

		strcat(reqBuffer, req->query);
	}
 
	strcat(reqBuffer, "\n");
}

