#ifndef p2p_event_loop
#define p2p_event_loop


#include "acc_utils.h"
#include "misc.h"
#include "zlog.h"
#include <liburing.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/types.h>

void event_loop_p2p(int serv_sock, struct io_uring *ring);

extern zlog_category_t *client_log;
extern int client_logging_enabled;



#endif


