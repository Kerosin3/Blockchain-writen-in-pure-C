#ifndef CLIENT

#define CLIENT

#include "../serdes/transaction.pb-c.h"
#include "../accounts/acc_utils.h"
#include "blockdata.h"
#include "merkle_process.h"
#include "block_process.h"
#include "zlog.h"
#include "ipc_messages_client.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <liburing.h>
#include <linux/io_uring.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
extern IpcMessage* buffer_transactions;

extern zlog_category_t *client_log;
extern int client_logging_enabled;
int setup_client_iouring();

#endif
