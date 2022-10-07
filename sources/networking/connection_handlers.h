#ifndef CONNECTIONHANDLERS
#define CONNECTIONHANDLERS

#define _GNU_SOURCE
#include "settings.h"
#include "setup.h"
#include "misc.h"
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <fcntl.h>
#include <liburing.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef enum
{
    FLAG_ACCEPT = 0,
    ASK_FOR_TRANSACTIONS, 
    WAIT_ANSWER_FOR_TRANSACTIONS,
    FLAG_SEND_ECHO,
    FLAG_SEND_TRANSACTIONS ,
    FLAG_READ ,
    FLAG_WRITE ,
    FLAG_ASK_MORE,
} flag_state;

// concat fd and state to uin64
u_int64_t make_request_data(int client_fd, flag_state flag);

void set_flags(int socket);
void handle_responce(struct io_uring *ring, int client_fd);

void add_ask_transaqtion_request(struct io_uring *ring, int client_fd);
void handle_request_transactions(struct io_uring *ring, int client_fd);
int request_data_client_fd(uint64_t request_data);

flag_state request_data_event_type(uint64_t request_data);

char *get_client_buffer(int client_fd);


void add_accept_request(struct io_uring *ring, int serverfd, struct sockaddr_in *a_client_adrd,
                        socklen_t *client_addr_len);
void add_read_request(struct io_uring *ring, int client_fd);

void add_write_request(struct io_uring *ring, int client_fd, size_t nbytes, bool more_data);

void handle_request(struct io_uring *ring, int client_fd, size_t n_read);
#endif
