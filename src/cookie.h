
/*
   OrionSocket - HTTP Request and Response handling Impl
   --------------------------------

   Author: Tiago Natel de Moura <tiago4orion@gmail.com>

   Copyright 2010, 2011 by Tiago Natel de Moura. All Rights Reserved.

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
#ifndef __ORION_COOKIE_H_
#define __ORION_COOKIE_H_

#include "debug.h"

#define ORION_DEBUG_COOKIE(c) \
        ORION_DEBUG("=== DEBUG COOKIE ===\n"); \
        ORION_DEBUG_SAFE("NAME=%s\n", c->name); \
        ORION_DEBUG_SAFE("VALUE=%s\n", c->value); \
        ORION_DEBUG_SAFE("COMMENT=%s\n", c->comment); \
        ORION_DEBUG_SAFE("COMMENTURL=%s\n", c->commentURL); \
        ORION_DEBUG_SAFE("DISCARD=%s\n", c->discard ? "ON" : "OFF"); \
        ORION_DEBUG_SAFE("PORT=%s\n", c->port); \
        ORION_DEBUG_SAFE("DOMAIN=%s\n", c->domain); \
        ORION_DEBUG_SAFE("PATH=%s\n", c->path); \
        ORION_DEBUG_SAFE("MAX-AGE=%s\n", c->max_age); \
        ORION_DEBUG_SAFE("SECURE=%s\n", c->secure ? "ON" : "OFF"); \
        ORION_DEBUG_SAFE("HTTPONLY=%s\n", c->httpOnly ? "ON" : "OFF"); \
        ORION_DEBUG_SAFE("VERSION=%s\n", c->version); \
        ORION_DEBUG_SAFE("EXPIRES=%s\n", c->expires);

/**
 * The orion_cookie structure holds one network cookie.
 *
 * From RFC2109, RFC2965 and Netscape Navigator Cookie Spec
 *
 * Cookies are informations that the HTTP protocol uses to maintain some
 * persistent information across requests.
 *
 * The minimum information of a cookie is a name and a value, but they can also
 * store more data. Below is list of all information a cookie can have.
 *
 * - Name\n
 *      The cookie's name.\n
 * - Value\n
 *      The cookie's value.\n
 * - Comment\n
 *      - Optional: A comment about the cookie.\n
 * - CommentURL\n
 *      - Optional: A URL about the cookie
 * - Discard\n
 *      - Optional: The Discard attribute instructs the user agent to
 *        discard the cookie unconditionally when the user agent 
 *        terminates. 
 * - Domain\n
 *      - Optional: Specifies the domain for which the cookie is valid.\n
 *      - Default behavior: The request-host.\n
 * - Max-Age\n
 *      - Optional: Defines the lifetime of the cookie, in seconds. If 
 *            Max-Age=0 then the browser should discard the cookie imediately.\n
 *      - Default Behavior: Discard the cookie when the user agent exit.\n
 * - Path\n
 *      - Optional: The subset of URLs to which this cookie applies.\n
 *      - Default Behavior: Defaults to the path of the request URL that 
 *            generated the Set-Cookie response, up to, but not including, the
 *            right-most /. \n
 * - Port\n
 *      - Optional: The Port attribute restricts the port to which a 
 *        cookie may be returned in a Cookie request header.  
 *        Note that the syntax REQUIREs quotes around the OPTIONAL 
 *        portlist even if there is only one portnum in portlist.
 * - Secure\n
 *      - Optional: The "Secure" attribute (with no value) directs the user
 *        agent to use only secure means to contact the server whenever it
 *        sends back this cookie.\n
 *      - Default behavior: If absent, the user agent may send the cookie 
 *        over an insecure channel.\n
 * - HttpOnly\n
 *      - Optional: A flag indicating that the cookie can not be 
 *        accessed on the client side scripts.
 * - Version\n
 *      - Required? Identifies which version of the cookie specification.\n
 * - Expires\n
 *      - Optional: The date for the cookie be discarded.\n
 *
 * @see orion_addCookie
 * @see orion_buildCookie
 * @see orion_initCookie
 * @see orion_setCookie
 */
typedef struct
{
	char* name;     /**< The cookie's name */
	char* value;    /**< The cookie's value. */
	char* comment;  /**< A comment about the cookie */
        char* commentURL; /**< A URL about the cookie. */
        _u8 discard;  /**< The user agent shall discard the cookie when 
                          terminate */
        char* port;     /**< List of ports where the cookie is valid. */
	char* domain;   /**< Specifies the domain for which the cookie is 
                          valid */
	char* max_age;  /**< Defines the lifetime of the cookie, in seconds */
	char* path;     /**< The subset of URLs to which this cookie applies */
	_u8 secure;   /**< The "Secure" attribute (with no value) directs the user
                        agent to use only secure means to contact the server 
                        whenever it sends back this cookie. */
        _u8 httpOnly;

	char* version;  /**< Identifies which version of the cookie specification */
	char* expires;	/**< The date for the cookie be discarded */
} orion_cookie;

enum cookie_av_token {
    COOKIE_AV_COMMENT = 1,
    COOKIE_AV_COMMENTURL,
    COOKIE_AV_DISCARD,
    COOKIE_AV_PORT,
    COOKIE_AV_DOMAIN,
    COOKIE_AV_MAXAGE,
    COOKIE_AV_PATH,
    COOKIE_AV_SECURE,
    COOKIE_AV_HTTPONLY,
    COOKIE_AV_VERSION,
    COOKIE_AV_EXPIRES
};


/**
 * Iniciliza a estrutura orion_cookie
 * @param orion_cookie** cookie
 * @return void
 */
extern void orion_initCookie(orion_cookie** cookie);

/**
 * Release all the memory allocated in "cookie".
 * 
 * To avoid memory leaks and other memory related problems, always call this
 * function after the connection ends.
 *
 * @see orion_initCookie
 * @param *cookie orion_cookie
 * @return void
 */
extern void orion_cleanupCookie(orion_cookie* cookie);

extern void orion_setCookie(orion_cookie *cookie, const char* name, const char* value, const char* comment, const char* domain, const char* max_age, const char* path, _u8 secure, _u8 httpOnly, const char* version, const char* expires);
extern void orion_parseCookie(orion_cookie*,const char*);
extern _u8 orion_getCookieAttrTk(const char*);
extern void orion_setCookieAttrTk(orion_cookie* c, _u8 attr, const char* value);

#endif /* __ORION_COOKIE_H_ */
