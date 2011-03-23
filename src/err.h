/*
   OrionSocket - Error Codes
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

#ifndef __ORION_SOCKET_ERR_
#define __ORION_SOCKET_ERR_

#define ORION_EAI_BADFLAGS		-1	/* Invalid value for `ai_flags' field.  */
#define ORION_EAI_NONAME		-2	/* NAME or SERVICE is unknown.  */
#define ORION_EAI_AGAIN			-3	/* Temporary failure in name resolution.  */
#define ORION_EAI_FAIL			-4	/* Non-recoverable failure in name res.  */
#define ORION_EAI_FAMILY		-6	/* `ai_family' not supported.  */
#define ORION_EAI_SOCKTYPE		-7	/* `ai_socktype' not supported.  */
#define ORION_EAI_SERVICE		-8	/* SERVICE not supported for `ai_socktype'.  */
#define ORION_EAI_MEMORY		-10	/* Memory allocation failure.  */
#define ORION_EAI_SYSTEM		-11	/* System error returned in `errno'.  */
#define ORION_EAI_OVERFLOW		-12	/* Argument buffer overflow.  */
#define ORION_EAI_NODATA		-5	/* No address associated with NAME.  */
#define ORION_EAI_ADDRFAMILY		-9	/* Address family for NAME not supported.  */
#define ORION_EAI_INPROGRESS		-100	/* Processing request in progress.  */
#define ORION_EAI_CANCELED		-101	/* Request canceled.  */
#define ORION_EAI_NOTCANCELED		-102	/* Request not canceled.  */
#define ORION_EAI_ALLDONE		-103	/* All requests done.  */
#define ORION_EAI_INTR			-104	/* Interrupted by a signal.  */
#define ORION_EAI_IDN_ENCODE		-105	/* IDN encoding failed.  */
#define ORION_SOCKET_ERR_UNKNOWN        -200
#define ORION_SOCKET_ERR_ALLOC          -201 
#define ORION_SOCKET_ERR_HTTPREQUEST    -202
#define ORION_SOCKET_INVALIDHOST        -203
#define ORION_SOCKET_INVALIDHTTPREQUEST -204
#define ORION_SOCKET_ADDR_NOTFOUND      -205
#define ORION_SOCKET_BIND_ERROR         -206
#define ORION_SOCKET_ACCEPT_ERROR       -207

void orion_socket_geterror(_i8 error, char* buffer);
void _orion_socket_geterror_macroname(_i8 error, char* buffer);

#endif /* __ORION_SOCKET_ERR_ */

