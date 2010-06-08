#ifndef __ORIONSOCKET_
#define __ORIONSOCKET_

#include <stdint.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif

#define ORIONSOCKET_OK              0x00

#define METHOD_GET                  0x01
#define METHOD_POST                 0x02
#define METHOD_TRACE                0x03
#define METHOD_PUT                  0x04
#define METHOD_DELETE               0x05

#define ORION_TCP_FAMILY            AF_INET
#define ORION_TCP_SOCKETTYPE       SOCK_STREAM

#define HTTP_PROTOCOL               "HTTP/1.1"
#define HTTP_REQUEST_MAXLENGTH      2048
#define HTTP_RESPONSE_LENGTH        1024
#define HTTP_BIG_RESPONSE           8192

#define DNS_MAXLENGTH               255
#define URL_MAXLENGTH               2048
#define IP_MAXLENGTH                16

#ifdef ORIONSOCKET_DEBUG
#define DEBUG_HTTPREQUEST(req)  do {  \
                                printf("DEBUGGING HTTP REQUEST:\n"); \
                                printf("Host: %s\n", req->address.domain); \
                                printf("Port: %d\n", req->address.port); \
                                printf("Method: %s\n", _getStrMethod(req->method)); \
                                int _orionIntdebug; \
                                for (_orionIntdebug = 0; _orionIntdebug < req->headerLen; _orionIntdebug++) \
                                    printf("%s: %s\n", req->header[_orionIntdebug].name, req->header[_orionIntdebug].value); \
                                for (_orionIntdebug = 0; _orionIntdebug < req->paramLen; _orionIntdebug++) \
                                    printf("%s=%s&", req->param[_orionIntdebug].name, req->param[_orionIntdebug].value); \
                                } while(0) 
#else
#define DEBUG_HTTPREQUEST(req)
#endif

typedef uint8_t _uint8;
typedef uint16_t _uint16;
typedef uint32_t _uint32;
    
// Estrutura Nome-Valor.
// Usada por toda a API.
typedef struct 
{
    char* name;
    char* value;
} nameValue;

// Armazena as informações do alvo
typedef struct 
{
    char domain[DNS_MAXLENGTH];
    char ip[IP_MAXLENGTH];
    _uint16 port;   // 0..65535
} address;

typedef struct
{
    address address;
    _uint8 method;
    char path[URL_MAXLENGTH];
    nameValue *header;
    uint8_t headerLen;
    nameValue *param;
    _uint16 paramLen;
    _uint16 options;
} httpRequest;

typedef struct
{
    address address;
    char *data;
} rawRequest;

extern void orionHTTPRequestInit(httpRequest *req);
extern void orionHTTPRequestCleanup(httpRequest *req);
extern _uint8 orionSetHttpDomain(httpRequest *req, const char* domain);
extern _uint8 orionSetHttpHeader(httpRequest *req, const char* name, const char* value);
extern void orionAssemblyHttpRequest(httpRequest *req, char* reqBuffer);
extern int orionTCPConnect(address);
extern _uint8 orionHTTPRequest(httpRequest *req, char** response);

#endif // __APISOCKET_
