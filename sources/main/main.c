#include <stdio.h>
#include <stdlib.h>
#include "activation.h"
#include "tests.h"
#include "start_server.h"

int main(int argc, char *argv[]){
	//tests();
	start_server(12345);
	return 0;
}
