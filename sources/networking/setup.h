#ifndef SETUP

#define SETUP
#include "settings.h"
#include "transaction.pb-c.h"
#include <dirent.h>
#include <liburing.h>
#include <netdb.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>



extern char *buffers; // buffers for incomn connections
extern size_t *buffer_lengths; //current buffer length
extern char *buffer_transactions; 
int setup_serv_sock(uint16_t port);

void destroy_buffers();
void setup_buffers();
void setup_iouring(struct io_uring *ring, int ncon, bool pooling);
inline _Noreturn void die(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
#endif
