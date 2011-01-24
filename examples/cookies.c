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
	orion_assembleCookie(c, "PHPSESSID=41afbb330591cdf781ca1441caab2f72; path=/; domain=.imasters.com.br");

		printf("%s\n", c->name);
	printf("%s\n", c->value);
	printf("%s\n", c->domain);
	printf("%s\n", c->path);
	printf("%s\n", c->proto != NULL ? c->proto : "");
	printf("%s\n", c->expires != NULL ? c->expires : "");

		orion_cleanupCookie(c);
	
	return 0;
}

