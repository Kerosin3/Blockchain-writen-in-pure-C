#ifndef p2p_event_loop
#define p2p_event_loop


#include "acc_utils.h"
#include "misc.h"
#include "p2p_setup.h"
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
#include "dhandlers_p2p.h"
#include "common.h"

/** @file p2p_event_loop.h
 * Setupping p2p io_uring
 */


typedef struct {
	struct io_uring *ring;
	int serv_sock;
} event_p2p_params_t;

/*!
 * @brief main event loop for p2p server 
 *****************************************************************************/
void event_loop_p2p(event_p2p_params_t*);

extern P2pIpcMessage__Status P2P_deserialize_STATUS(char *buff, unsigned len);
extern void die(char*);
extern zlog_category_t *client_log;
extern int client_logging_enabled;

extern zlog_category_t *p2p_log;
extern int p2p_logging_enabled;


#endif


