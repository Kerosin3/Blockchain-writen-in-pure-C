#include <stdio.h>
#include <stdlib.h>
#include "setup_client.h"
#include <zlog.h>

const char* client_conf_logfile = "/home/ker0/test/prj/sources/logging/zlog.conf";
int client_logging_enabled;
zlog_category_t *client_log;

int main(int argc, char *argv[]){
	client_conf_logfile = 0;

	int rc;

 	rc = zlog_init(client_conf_logfile);
	if (rc) {
		printf("logging failed\n");
	}
	
	atexit(zlog_fini);

	client_log = zlog_get_category("log_client");
	if (!client_log) {
		printf("failed init logging category!\n");
		zlog_fini();
	}
#if(WRITE_LOG == 1) 
	zlog_info(client_log, "client logging started!");
	client_logging_enabled = 1;
#endif
	setup_client_iouring();
	return 0;
}
