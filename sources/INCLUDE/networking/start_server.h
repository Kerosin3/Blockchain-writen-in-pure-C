#ifndef SERVER
#define SERVER

#include "event_loop.h"
#include "setup.h"
#include <linux/io_uring.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief this func is starting the ui_uring bases server
 * @param port port used to launch the server
 * **********************************************************/
void start_server(uint16_t port);

extern int logging_enabled;
/**
 * @note zlog global var to write log
 * *********************************************************/
extern zlog_category_t *server_log;
#endif
