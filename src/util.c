/*
   OrionSocket - Util implementation
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
#include "util.h"
#include "socket.h"
#include <stdlib.h>
#include <string.h>

void * orion_realloc(void *ptr, size_t size)
{
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}

const char* orion_getStrMethod(_uint8 method)
{
    switch (method)
    {
    case METHOD_GET:
        return "GET";
    case METHOD_POST:
        return "POST";
    case METHOD_TRACE:
        return "TRACE";
    case METHOD_PUT:
        return "PUT";
    case METHOD_DELETE:
        return "DELETE";
    default:
        return "UNKNOWN";
    }
}

char* orion_getNextLine(char* buffer)
{
    int pos_endl = 0;
    char* line = NULL;
    pos_endl = orion_linearSearchChar(buffer, '\n');
    if (pos_endl > 0)
    {
        buffer[pos_endl] = '\0';
        line = buffer;
        
        return line;
    } else
        return NULL;
}

int orion_linearSearchChar(const char* buffer, char c)
{   
    int i;
    for (i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == c)
            return i;
    }
    
    return -1;
}

