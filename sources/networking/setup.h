#ifndef SETUP

#define SETUP

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


#define MAX_CONNECTIONS 1000
#define BUFFER_SIZE 4096

extern char *buffers; // buffers for incomn connections
extern size_t *buffer_lengths; //current buffer length
int setup_serv_sock(uint16_t port);

inline _Noreturn void die(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
#endif
