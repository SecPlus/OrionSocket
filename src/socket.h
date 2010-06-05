#ifndef __ORIONSOCKET_
#define __ORIONSOCKET_

#include <stdint.h>

#define METHOD_GET                  0x01
#define METHOD_POST                 0x02
#define METHOD_TRACE                0x03
#define METHOD_PUT                  0x04
#define METHOD_DELETE               0x05

#define HTTP_PROTOCOL               "HTTP/1.1"
#define HTTP_REQUEST_MAXLENGTH      2048
#define HTTP_RESPONSE_LENGTH        1024
#define HTTP_BIG_RESPONSE           8192

#define DNS_MAXLENGTH               255

#define DEBUG_HTTPREQUEST       printf("Host: %s\n", req->victim->host); \
                                printf("Port: %d\n", req->victim->port); \

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
    _uint16 port;   // 0..65535
} address;

typedef struct
{
    address *victim;
    _uint8 method;
    char* path;
    nameValue *header;
    uint8_t headerLen;
    nameValue *param;
    _uint16 paramLen;
} httpRequest;

typedef struct
{
    address *victim;
    char *data;
} rawRequest;

extern _uint8 setHttpHeader(httpRequest *req, const char* name, const char* value);
extern void httpRequestCleanup(httpRequest *req);
extern void assemblyHttpRequest(httpRequest *req, char* reqBuffer);
extern int orionTCPConnect(address);
extern _uint8 orionHTTPRequest(httpRequest *req, char** response);

#endif // __APISOCKET_
