#ifndef CONNECTIONHANDLERS
#define CONNECTIONHANDLERS

#define _GNU_SOURCE
#include "acc_utils.h"
#include "data_handlers.h"
#include "ipcmessages.h"
#include "layering.h"
#include "misc.h"
#include "serdes.h"
#include "settings.h"
#include "setup.h"
#include "start_server.h"
#include "zlog.h"
#include <asm-generic/errno-base.h>
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

extern circ_buf_t CBUF;

typedef enum
{
    FLAG_ACCEPT = 0,
    TEST_RESPONSE,
    READ_RESPONSE,
    WAIT_ACKNOWLEDGEMENT,
    FLAG_SEND_TRANSACTIONS,
    FLAG_CLOSE_CONNECTION,
    FLAG_READ,
    FLAG_WRITE,
} flag_state;
// bunch of function to process even loop events
signed_message_t *get_signed_message_buffer(int client_fd);
void FINISH_SENDING(struct io_uring *ring, int client_fd);
void request_SEND_STATUS(struct io_uring *ring, int client_fd, IpcMessage__Status status);
u_int64_t make_request_data(int client_fd, flag_state flag);
IpcMessage *get_ipc_msg_buffer(int client_fd);
void READ_STATUS_RESPONSE(struct io_uring *ring, int client_fd);
void set_flags(int socket);
void WHETHER_ACK_OK(struct io_uring *ring, int client_fd);
void request_ASK_NEED_MSG(struct io_uring *ring, int client_fd);
void handle_request_transactions(struct io_uring *ring, int client_fd);
int request_data_client_fd(uint64_t request_data);
flag_state request_data_event_type(uint64_t request_data);
char *get_client_buffer(int client_fd);
void handle_response_NEED_MORE_MSG(struct io_uring *ring, int client_fd);
void add_accept_request(struct io_uring *ring, int serverfd, struct sockaddr_in *a_client_adrd,socklen_t *client_addr_len);
void add_read_request(struct io_uring *ring, int client_fd);
void add_write_request(struct io_uring *ring, int client_fd, size_t nbytes, bool more_data);
void handle_request(struct io_uring *ring, int client_fd, size_t n_read);
#endif
