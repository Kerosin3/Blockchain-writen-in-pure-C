#ifndef P2PLISTEN

#define P2PLISTEN

#include "p2p_settings.h"
#include <dirent.h>
#include <fcntl.h>
#include <liburing.h>
#include "arpa/inet.h"
#include <linux/limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <threads.h>
#include "zlog.h"
#include "p2ptransaction.pb-c.h"
#include "common.h"
void setup_p2p_listening(char*);

P2pIpcMessage__Status P2P_deserialize_STATUS(char *buff, unsigned len);
extern size_t P2P_send_status(P2pIpcMessage__Status STATUS,void* buf_out);

extern zlog_category_t *p2p_log;
extern int p2p_logging_enabled;


#endif
