#ifndef CLIENT

#define CLIENT

#include "acc_utils.h"
#include "transaction.pb-c.h"
#include "block_process.h"
#include "blockdata.h"
#include "ipc_messages_client.h"
#include <threads.h>
#include "merkle_process.h"
#include "zlog.h"
#include <arpa/inet.h>
#include <errno.h>
#include <liburing.h>
#include <linux/io_uring.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/** @file setup_client.h
 * Setupping client io_uring
 */

/*!
 * @brief starting client, listen port 12345
 * @param port port to start on
 *****************************************************************************/
int setup_client_iouring();

extern IpcMessage *buffer_transactions;

extern zlog_category_t *client_log;
extern int client_logging_enabled;

extern zlog_category_t *p2p_log;
extern int p2p_logging_enabled;


#endif
