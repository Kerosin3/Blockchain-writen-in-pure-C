#ifndef SERVER
#define SERVER

#include "event_loop.h"
#include "setup.h"
#include <linux/io_uring.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
void start_server(uint16_t port);

extern int server_logging_enabled;
extern zlog_category_t *server_log;
#endif
