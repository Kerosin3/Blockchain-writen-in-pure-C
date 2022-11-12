#ifndef P2PLISTEN

#define P2PLISTEN

#include "arpa/inet.h"
#include "blockdata.h"
#include "common.h"
#include "p2p_settings.h"
#include "p2ptransaction.pb-c.h"
#include "zlog.h"
#include <dirent.h>
#include <fcntl.h>
#include <liburing.h>
#include <linux/limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <threads.h>
#include <unistd.h>
void setup_p2p_listening(char *);

P2pIpcMessage__Status P2P_deserialize_STATUS(char *buff, unsigned len);
extern size_t P2P_send_status(P2pIpcMessage__Status STATUS, void *buf_out);
extern block_t *deserialize_block(void *buf_in, unsigned len);

extern zlog_category_t *p2p_log;
extern int p2p_logging_enabled;

#endif
