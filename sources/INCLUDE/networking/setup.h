#ifndef SETUP

#define SETUP
#include "../accounts/acc_utils.h"
#include "../serdes/transaction.pb-c.h"
#include "settings.h"
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
#include <unistd.h>

extern char *buffers;          // buffers for incomn connections
extern size_t *buffer_lengths; // current buffer length
extern IpcMessage *buffer_transactions;
extern signed_message_t *buffer_signed_message;
extern size_t *beffer_sended_N;
int setup_serv_sock(uint16_t port);


extern signed_message_t** ring_data_buf;

void destroy_buffers();
void setup_buffers();
void setup_iouring(struct io_uring *ring, int ncon, bool pooling);
inline _Noreturn void die(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
#endif
