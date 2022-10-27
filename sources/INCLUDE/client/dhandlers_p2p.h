#ifndef dhandlers_p2p

#define dhandlers_p2p

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "p2p_settings.h"
#include <errno.h>
#include <fcntl.h>
#include <liburing.h>
#include <limits.h>
#include <netinet/in.h>
#include <protobuf-c/protobuf-c.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <zlog.h>

typedef enum
{
    FLAG_ACCEPT = 0,
} STATE_P2P;


extern zlog_category_t *p2p_log;
extern int p2p_logging_enabled;


extern char *buffers;                  
extern size_t *buffer_lengths;          
extern size_t *buffer_sended_N;

void add_accept_request(struct io_uring *ring, int serverfd, struct sockaddr_in *a_client_adrd,
                        socklen_t *client_addr_len);

void set_flags(int socket);
STATE_P2P request_data_event_type(uint64_t request_data);
int request_data_client_fd(uint64_t request_data);
#endif
