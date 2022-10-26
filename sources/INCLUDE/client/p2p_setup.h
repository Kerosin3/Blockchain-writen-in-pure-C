#ifndef p2p_setup

#define p2p_setup
#include "p2p_settings.h"
#include "blockdata.h"
#include "transaction.pb-c.h"
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
#include <threads.h>

extern char *buffers;                  
extern size_t *buffer_lengths;          
extern size_t *buffer_sended_N;


void destroy_buffers();
void setup_buffers();

void teardown_server_sock(int servsock);
int setup_serv_sock(uint16_t port);
void setup_iouring(struct io_uring *ring,  bool pooling);

#endif
