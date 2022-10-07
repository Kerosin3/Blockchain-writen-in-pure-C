#ifndef SERVER
#define SERVER

#include "setup.h"
#include "event_loop.h"
#include <linux/io_uring.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
void start_server(uint16_t port);

#endif
