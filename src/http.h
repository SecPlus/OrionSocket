/*
   OrionSocket - HTTP Request and Response handling
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
#ifndef __ORIONSOCKET_HTTP_H_
#define __ORIONSOCKET_HTTP_H_

#include "types.h"
#include "api.h"
#include "socket.h"

/**
 * Options
 */
#define ORION_OPTDEBUG_REQUEST      0x01
#define ORION_OPTDEBUG_RESPONSE     0x02
#define ORION_OPTDEBUG_PROGRESS     0x04
#define ORION_OPTRESPONSE_DETAIL    0x08

#define ORION_HTTP_PROTOCOL         "HTTP/1.1"

#define ORION_HTTP_PROTOCOL_1_0     0x00
#define ORION_HTTP_PROTOCOL_1_1     0x01
#define ORION_HTTP_PROTOCOL_UNKNOWN 0x02

/**
 * Response lengths
 */
#define ORION_HTTP_REQUEST_MAXLENGTH      2048
#define ORION_HTTP_RESPONSE_LENGTH        1024
#define ORION_HTTP_BIG_RESPONSE           8192

/**
 * Request methods
 */
#define ORION_METHOD_GET            0x01
#define ORION_METHOD_POST           0x02
#define ORION_METHOD_TRACE          0x03
#define ORION_METHOD_PUT            0x04
#define ORION_METHOD_DELETE         0x05

/**
 * MACROS
 */
#define ORION_GETPARTCOOKIE(part,tam) \
			do { \
			bufHandle += tam+1;   \
			sz = strlen(bufHandle); \
			for (i = 0; i < sz && bufHandle[i] != ';'; i++); \
			if (i == sz-1) { \
			    free(lineBuffer); \
				return; \
		    } \
			bufHandle[i] = '\0'; \
			cookie->part = strdup(bufHandle); \
			bufHandle += i + 2; \
			} while(0)


/**
 * Estrutura de Dados
 */

// Cookie struct
typedef struct
{
	char* name;
	char* value;
	char* domain;
	char* path;
	char* proto;
	char* expires;	
} orion_cookie;


// orionHttpRequest structure
typedef struct
{
    char* host;         /* Host Target          */
    _uint16 port;       /* Port Target          */
    _uint8 method;      /* HTTP Method          */
    char* path;         /* URL Path             */
    char* file_ext;     /* File extension       */
    char* query;        /* Query String         */

    nameValue *header;  /* HTTP    Headers      */
    _uint8 headerLen;   /* Number of headers    */

    orion_cookie *cookie;  /* Array of cookies     */
    _uint8 cookieLen;   /* Number of cookies    */

    _uint16 option;    /* Extra Options        */
} orion_httpRequest;

typedef struct
{
    _uint8 version;         /* HTTP version. 1.0 || 1.1 */
    _uint16 code;            /* Status Code              */
    char* message;          /* Server Message           */
    char* serverName;       /* Server Name              */
    char* date;             /* Date                     */
    char* expires;          /* Expires time             */
    char* location;         /* Location.                */
    char* mime_version;     /* MIME-VERSION             */
    char* content_type;     /* Content-Type             */
    char* charset;          /* Charset                  */
    _uint32 content_length;   /* Length of the content;   */

    nameValue *header;      /* HTTP Headers             */   
    _uint8 headerLen;       /* Number of headers        */

    orion_cookie *cookie;      /* Set Cookie               */
    _uint8 cookieLen;       /* Number of cookies        */
    
    char* body;             /* Body of the response     */
} orion_httpResponse;

/*******************************************************************************
 * API FOR REQUEST                                                             *
 ******************************************************************************/
 
/**
 * Inicializa e aloca a memória necessária para a estrutura orion_httpRequest.
 * TODO : Remover esse cara
 *
 * @deprecated
 * @param orion_httpRequest **req
 * @return void
 */
extern void orion_httpRequestInit(orion_httpRequest **req);

/**
 * Inicializa e aloca a memória necessária para a estrutura orion_httpRequest.
 *
 * @param orion_httpRequest **req
 * @return void
 */
extern void orion_initHttpRequest(orion_httpRequest **req);

/**
 * Libera a memória alocada por orion_httpRequestInit
 * TODO : Remover esse cara
 *
 * @deprecated
 * @param orion_httpRequest *req
 * @return void
 */
extern void orion_httpRequestCleanup(orion_httpRequest *req);

/**
 * Libera a memória alocada por orion_initHttpRequest
 *
 * @param orion_httpRequest *req
 * @return void
 */
extern void orion_cleanupHttpRequest(orion_httpRequest *req);

extern void orion_cleanupCookie(orion_cookie* cookie);
/**
 * Configura o host e a porta para a conexão HTTP.
 *
 * @param orion_httpRequest *req
 * @param const char* host
 * @param _uint16 port
 */
extern void orion_setHttpRequestHost(orion_httpRequest *req, const char* host, _uint16 port);

/**
 * Configura o Path da requisição.
 * Por exemplo, na URL http://www.bugsec.com.br/tools/
 * temos que PATH=/tools/shells/c99.txt
 * 
 * @param orion_httpRequest *req
 * @param const char* path
 * @return void
 */ 
extern void orion_setHttpRequestPath(orion_httpRequest *req, const char* path);

/**
 * Configura um header HTTP.
 * 
 * @param orion_httpRequest *req
 * @param const char* name
 * @param const char* value
 * @return _uint8
 */
extern _uint8 orion_setHttpRequestHeader(orion_httpRequest *req, const char* name, const char* value);

/**
 * Configura uma opção de controle para a requisição.
 * 
 * @param orion_httpRequest* req
 * @param _uint8 option
 * @return void
 */
extern void orion_setHttpRequestOption(orion_httpRequest* req, _uint16 option);

/**
 * Monta a requisição HTTP a partir da estrutura orion_httpRequest
 * previamente montada.
 *
 * @param orion_httpRequest *req
 * @param char* reqBuffer
 * @return void
 */
extern void orion_assembleHttpRequest(orion_httpRequest *req, char* reqBuffer);

/**
 * Monta a estrutura orion_httpResponse a partir do cabeçalho retornado pelo
 * servidor.
 * 
 * @param orion_httpResponse *res
 * @param char* resBuffer
 * @return void
 */
extern void orion_assembleHttpResponse(orion_httpResponse *res, char* resBuffer);

/**
 * Executa a requisição HTTP no host alvo.
 * Toda a resposta do host é armazenada na variável response.
 * Essa função não retornará até todos os dados forem obtidos.
 * Se voce deseja ter mais controle do stream de dados, utilize
 * a função orion_httpGet().
 *
 * @param orion_httpRequest *req
 * @param char* response
 * @return _uint8
 */
extern _uint8 orion_httpRequestPerform(orion_httpRequest *req, char** response);

/**
 * Executa a requisição HTTP no host alvo.
 * A cada count bytes retornados pelo servidor, é chamada a função callback
 * passando a string e o numero de bytes lidos.
 *
 * @param orion_httpRequest* req
 * @param void (*callback)(char*, _uint32)
 * @param _uint32 count
 * @return _uint8
 */
extern _uint8 orion_httpGet(orion_httpRequest* req, void (*callback)(char*,_uint32), _uint32 count);

extern void orion_initCookie(orion_cookie** cookie);
extern void orion_cleanupCookie(orion_cookie* cookie);
extern void orion_setCookie(orion_cookie *cookie, const char* name, const char* value, const char* domain, const char* path, const char* proto, const char* expires);
extern _uint8 orion_addCookie(orion_httpResponse* response, orion_cookie* cookie);

/*******************************************************************************
 * API FOR RESPONSE                                                            *
 ******************************************************************************/

/**
 * Initialize and allocate memory for the structure orion_httpResponse
 *
 * @param orion_httpResponse
 * @return void
 */
extern void orion_initHttpResponse(orion_httpResponse **res);

/**
 * Free the memory allocated by orion_initHttpResponse
 * 
 * @param orion_httpResponse
 * @return void
 */
extern void orion_cleanupHttpResponse(orion_httpResponse* res);

/**
 * Add a header to the struct orion_httpResponse
 *
 * @param orion_httpResponse
 * @return void
 */
extern void orion_setHttpResponseHeader(orion_httpResponse* res, const char* name, const char* value);
extern void orion_parseResponseLine(orion_httpResponse* res, char* line);
extern void orion_assembleHttpResponse(orion_httpResponse *res, char* line);
extern void orion_assembleCookie(orion_cookie* cookie, char* lineBuffer);
extern _uint8 orion_httpReqRes(orion_httpRequest* req, orion_httpResponse** res);

#endif // __ORIONSOCKET_HTTP_H_

