#include "start_server.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    #ifdef TESTMODE
	printf("TESTMODE is ENABLED\n");
	printf("server and client app are not supposed to work\n");
    	tests();
	return 0;
    #endif

    start_server(12345);

    return 0;
}
