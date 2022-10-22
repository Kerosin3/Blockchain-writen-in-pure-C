#ifndef EVENT_LOOP
#define EVENT_LOOP

#include "../accounts/acc_utils.h"
#include "connection_handlers.h"
#include "zlog.h"
#include "start_server.h"
#include "data_handlers.h"
#include "misc.h"
#include "setup.h"
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

/**
 * @brief a buffer of N messages that are to send to connected clients
 * @note currently N is hardcoded and equals 512
 * *********************************************************************/
//extern circ_buf_t CBUF;

/**
 *@brief used to start main server loop
 */
void event_loop(int serv_sock, struct io_uring *ring);

#endif
