#ifndef SETUP

#define SETUP
#include "acc_utils.h"
#include "transaction.pb-c.h"
#include "data_handlers.h"
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
extern signed_message_t **ring_data_buf;


int setup_serv_sock(uint16_t port);
void destroy_buffers();
void setup_buffers();
/**
 * @brief setup server for sending signed messages
 *
 * @param pooling works only in root mode 
 */
void setup_iouring(struct io_uring *ring, int ncon, bool pooling);

inline _Noreturn void die(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
#endif
