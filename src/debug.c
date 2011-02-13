/*
   OrionSocket - Debug Implementation
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
 
 #include "debug.h"
 
 void orion_dump(const unsigned char* data, const unsigned int length)
 {
    unsigned char byte;
    unsigned int i, j;
    
    for (i = 0; i < length; i++)
    {
        byte = data[i];
        printf("%02x ", data[i]);
        if (((i%16)==15) || (i == length-1))
        {
            for (j = 0; j < 15-(i%16); j++)
                printf("   ");
            printf("| ");
            
            for (j = (i-(i%16)); j <= i; j++)
            {
                byte = data[j];
                if ((byte > 31) && (byte < 127)) // char a ser impresso
                    printf("%c", byte);
                else
                    printf(".");
            }
            
            printf("\n");
        }
    }
 }
 
