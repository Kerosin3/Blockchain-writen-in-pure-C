#include "start_server.h"



struct io_uring ring;

const char* server_conf_logfile = "/home/ker0/test/prj/sources/logging/zlog.conf";

circ_buf_t CBUF;
int logging_enabled;
zlog_category_t *server_log;
/**
 * @brief used to start server that sends tst messages
 * *******************************************************************************/
void start_server(uint16_t port)
{
    logging_enabled = 0;
    setup_buffers(MAX_CONNECTIONS);      // establish buffers
    int serv_fd = setup_serv_sock(port); // set server fd
    printf(">>launching server on a port: %d<<\n", port);
    setup_iouring(&ring, MAX_CONNECTIONS, false);
    signal(SIGPIPE, SIG_IGN);
    CBUF =  create_circ_buf();
	

	int rc;

 	rc = zlog_init(server_conf_logfile);
	if (rc) {
		printf("logging failed\n");
	}
	
	atexit(zlog_fini);

	server_log = zlog_get_category("my_log_f");
	if (!server_log) {
		printf("failed init logging category!\n");
		zlog_fini();
	}

#if(WRITE_LOG == 1) 
    logging_enabled = 1;
    zlog_info(server_log, "logging service for server enabled"); 
#endif
    event_loop(serv_fd, &ring);

    destroy_buffers();
}
