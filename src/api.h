/*
   OrionSocket - API Header
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
#ifndef __ORIONSOCKET_API_H_
#define __ORIONSOCKET_API_H_

// Structures
#define orion_httpRequest               orion_httpRequest

// API Functions
#define orion_httpRequestInit           orion_httpRequestInit
#define orion_httpRequestCleanup        orion_httpRequestCleanup
#define orion_setHttpRequestHeader      orion_setHttpRequestHeader
#define orion_setHttpRequestHost        orion_setHttpRequestHost
#define orion_assemblyHttpRequest       orion_assemblyHttpRequest
#define orion_tcpConnect                orion_tcpConnect
#define orion_httpRequestPerform        orion_httpRequestPerform
#define orion_getHostByName             orion_getHostByName
#define orion_getDomainByAddr           orion_getDomainByAddr
#define orion_getDomain                 orion_getDomain

#endif // __ORIONSOCKET_API_H_
