#include <stdio.h>
#include <orion/socket/http.h>

void parseResponse(orion_httpResponse* res, char* line);

int main()
{
	orion_httpResponse* res = NULL;
	orion_initHttpResponse(&res);
	parseResponse(res, "Date: 13/02/2011");
}

void parseResponse(orion_httpResponse* res, char* line)
{

}

