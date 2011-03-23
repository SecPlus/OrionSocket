#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "http.h"
#include "types.h"
#include "cookie.h"

void orion_initCookie(orion_cookie** cookie2)
{
    orion_cookie* cookie = (orion_cookie *) malloc(sizeof(orion_cookie));

    if (!cookie)
    {
#ifdef ORION_SOCKET_DEBUG
        fprintf(stderr, "[ERROR] Error allocating memory..."ORION_SOCKETNL);
#endif
        return;
    }

    cookie->name = NULL;
    cookie->value = NULL;
    cookie->comment = NULL;
    cookie->commentURL = NULL;
    cookie->discard = 0;
    cookie->port = NULL;
    cookie->domain = NULL;
    cookie->max_age = NULL;
    cookie->path = NULL;
    cookie->secure = 0;
    cookie->httpOnly = 0;
    cookie->version = NULL;
    cookie->expires = NULL;

    *cookie2 = cookie;
}

void orion_setCookie(    orion_cookie* cookie, 
                        const char* name, 
            			const char* value, 
                        const char* comment,
                        const char* domain, 
                        const char* max_age,
                        const char* path,                        
                        _u8 secure, 
                        _u8 httpOnly,
                        const char* version,
                        const char* expires)
{    
    if (!strlen(name))
        return;

    cookie->name = strdup(name);
    cookie->value = strdup(value);
    cookie->value = strdup(comment);
    cookie->domain = strdup(domain);
    cookie->max_age = strdup(max_age);
    cookie->path = strdup(path);
    cookie->secure = secure;
    cookie->httpOnly = httpOnly;
    cookie->version = strdup(version);
    cookie->expires = strdup(expires);
}

_i16 orion_addCookie(orion_httpResponse *res, orion_cookie* cookie)
{
    _u8 len = res->cookieLen;
    res->cookie = (orion_cookie*) orion_realloc(res->cookie, sizeof(orion_cookie)*(len+1));    

    if (!res->cookie)
    {
#ifdef ORION_SOCKET_DEBUG
        fprintf(stderr, "[ERROR] Error allocating memory..."ORION_SOCKETNL);
#endif
        return ORION_SOCKET_ERR_ALLOC;
    }

    res->cookie[len] = *cookie;
    res->cookieLen++;

    return ORION_OK;
}

void orion_cleanupCookie(orion_cookie* cookie)
{
    if (cookie)
    {
        ORIONFREE(cookie->name);
        ORIONFREE(cookie->value);
        ORIONFREE(cookie->comment);
        ORIONFREE(cookie->commentURL);
        ORIONFREE(cookie->port);
        ORIONFREE(cookie->domain);
        ORIONFREE(cookie->path);
        ORIONFREE(cookie->expires);
        cookie->secure = 0;
        cookie->httpOnly = 0;
        cookie->discard = 0;
    }

    ORIONFREE(cookie);
}

_u8 orion_getCookieAttrTk(const char* opt)
{
    _u8 i;

    typedef struct {
        char name[12];
        _u8 attr;
    } cookie_av2;

#define N_COOKIE_AV     11

    /**
     * Structure for optimized code.
     * In this mode we have less string comparison routines
     */
    static cookie_av2 cookiePair[N_COOKIE_AV] = {
        {"comment",     COOKIE_AV_COMMENT},
        {"commenturl",  COOKIE_AV_COMMENTURL},
        {"discard",     COOKIE_AV_DISCARD},
        {"port",        COOKIE_AV_PORT},
        {"domain",      COOKIE_AV_DOMAIN},
        {"max-age",     COOKIE_AV_MAXAGE},
        {"path",        COOKIE_AV_PATH},
        {"secure",      COOKIE_AV_SECURE},
        {"httpOnly",    COOKIE_AV_HTTPONLY}, 
        {"version",     COOKIE_AV_VERSION},
        {"expires",     COOKIE_AV_EXPIRES}
    };

    for (i = 0; i < N_COOKIE_AV; i++)
    {
        if (!strcasecmp(cookiePair[i].name, opt))
            return cookiePair[i].attr;            
    }

    return -1;
}

void orion_setCookieAttrTk(orion_cookie* c, _u8 attr, const char* value)
{
    switch (attr)
    {
    case COOKIE_AV_COMMENT:     c->comment = strdup(value);     break;
    case COOKIE_AV_COMMENTURL:  c->commentURL = strdup(value);  break;
    case COOKIE_AV_DISCARD:     c->discard = 1;                 break;
    case COOKIE_AV_PORT:   c->port = strdup(value);             break;
    case COOKIE_AV_DOMAIN:      c->domain = strdup(value);      break;
    case COOKIE_AV_PATH:        c->path = strdup(value);        break;
    case COOKIE_AV_MAXAGE:      c->max_age = strdup(value);     break;
    case COOKIE_AV_SECURE:      c->secure = 1;                  break;
    case COOKIE_AV_VERSION:     c->version = strdup(value);     break;
    case COOKIE_AV_EXPIRES:     c->expires = strdup(value);     break;
    case COOKIE_AV_HTTPONLY:    c->httpOnly = 1;                break;
    }
}

void orion_parseCookie(orion_cookie* c, const char* cookie)
{
    char delim, ck, *buffer = strdup(cookie);
    _i8 token, done = 0, cookie_tk = 0;
    ssize_t i;

    if (!strlen(buffer))
        return;
    /**
     * Parsing Set-cookie string according with RFC 2109
     *
     * NAME=VALUE SHALL appear in the begin
     */
    i = strcspn(buffer, "=;");
    
    delim = buffer[i];
    buffer[i] = '\0';
    c->name = strdup(buffer);
#ifdef DEBUG
    ORION_DEBUG("NAME=%s\n", c->name);
#endif

    buffer += i + 1;

    if (delim == '=')
    {
        /* getting the value */
        i = strcspn(buffer, ";");
        if (i > 0)
        {
            delim = buffer[i];
            buffer[i] = '\0';
            c->value = strdup(buffer);
#ifdef DEBUG
            ORION_DEBUG("VALUE=%s\n", c->value);
#endif
            buffer += i + 1;
        }
    } 
    
    /* getting other options in any order */

/*#define T_INIT    0x01 */
#define T_ENDOPT  0x02
#define T_EQUAL   0x04
#define T_NAMEVAL 0x08

    token = T_NAMEVAL;
    i = 0;
    while (!done)
    {
        ck = buffer[i];
        switch (ck)
        {
        case '\0':
            /* end of string, getting what we need and exit ... */
            done = 1;
        case ';':
            /* ensures that the "name" was already caught. */
            if (token == (T_NAMEVAL | T_EQUAL))
            {
                buffer[i] = '\0';

#ifdef ORION_SOCKET_DEBUG
                ORION_DEBUG("VALUE FOUND1: %s\n", buffer);
#endif

                orion_setCookieAttrTk(c, cookie_tk, buffer);
                cookie_tk = 0;
                buffer += i + 1;
                i = 0;
            } else 
            if (token == T_NAMEVAL) 
            {
                buffer[i] = '\0';

                if (strlen(buffer) && (cookie_tk = orion_getCookieAttrTk(orion_ltrim(buffer))) == -1)
                {
#ifdef ORION_SOCKET_DEBUG
                    ORION_DEBUG("1 - Cookie attribute '%s' inexistent in Cookie RFC 2109, RFC 2965 and Netscape Navigator Cookie Specs ... WTF\n", buffer);
#endif
                    return;
                }

                orion_setCookieAttrTk(c, cookie_tk, NULL);
                cookie_tk = 0;

                buffer += i + 1;
                i = 0;
            }
            token = T_ENDOPT;
            break;
        case '=':
            if (token == T_NAMEVAL)
            {
                buffer[i] = '\0';
                
                if ((cookie_tk = orion_getCookieAttrTk(orion_ltrim(buffer))) == -1)
                {
#ifdef ORION_SOCKET_DEBUG
                    ORION_DEBUG("2 - Cookie attribute '%s' inexistent in Cookie RFC 2109 ... WTF\n", buffer);
#endif
                    return;
                }

                buffer += i + 1;
                i = 0;
                token = T_NAMEVAL | T_EQUAL;
            } else
                token = T_EQUAL;

            break;
        default:
            token = token == T_ENDOPT ? T_NAMEVAL : token | T_NAMEVAL;
        }

        i++;
    }
}
