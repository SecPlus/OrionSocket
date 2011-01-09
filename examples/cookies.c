#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <orion/socket/http.h>


void assembleCookie(orion_cookie* c, char* s);
int main()
{
	orion_cookie *c = NULL;

	orion_initCookie(&c);

//	assembleCookie(c, "PREF=ID=e52d156f7713a539:FF=0:TM=1294551749:LM=1294551749:S=oJ5bYafPIDsLW2zc; expires=Tue, 08-Jan-2013 05:42:29 GMT; path=/; domain=.google.com");
	assembleCookie(c, "PHPSESSID=41afbb330591cdf781ca1441caab2f72; path=/; domain=.imasters.com.br");

		printf("%s\n", c->name);
	printf("%s\n", c->value);
	printf("%s\n", c->domain);
	printf("%s\n", c->path);
	printf("%s\n", c->proto);
	printf("%s\n", c->expires);

		orion_cleanupCookie(c);
	
	return 0;
}

void assembleCookie(orion_cookie* cookie, char* line)
{
	char* lineBuffer = strdup(line);
	char* bufHandle = lineBuffer;
	int i, sz = strlen(lineBuffer);

	for (i = 0; i < sz && bufHandle[i] != '='; i++);

	if (i == sz-1)
		return;

	bufHandle[i] = '\0';

	cookie->name = strdup(bufHandle);
	bufHandle += i+1;

	sz = strlen(bufHandle);
	for (i = 0; i < sz && bufHandle[i] != ';'; i++);

	if (i == sz-1)
		return;

	bufHandle[i] = '\0';

	cookie->value = strdup(bufHandle);
	bufHandle += i + 2;

	_uint8 done = 0;

	while (!done)
	{
		if (!strncasecmp("path", bufHandle, 4))
		{
			ORION_GETPARTCOOKIE(path,4);
			
		} else if (!strncasecmp("expires", bufHandle, 7))
		{
			ORION_GETPARTCOOKIE(expires,7);
		} else if (!strncasecmp("domain", bufHandle, 6))
		{
			ORION_GETPARTCOOKIE(domain,6);
		} else 
			done = 1;
	}

	sz = strlen(bufHandle);

	if (sz > 0)
	{
		for (i = 0; i < sz && bufHandle[i] != ';'; i++);

		if (i > 0)
		{
			bufHandle[i] = '\0';
			cookie->proto = strdup(bufHandle);
		}
	}
	
	free(lineBuffer);
	
}


