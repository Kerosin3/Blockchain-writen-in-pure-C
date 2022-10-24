#ifndef SERVER
#define SERVER

#include "event_loop.h"
#include "setup.h"
#include <linux/io_uring.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
/** @file start_server.h
 * Setupping server IU_URING
 */

/*!
 * @brief starting server on specified port
 * @param port port to start on
 *****************************************************************************/
void start_server(uint16_t port);

/**
 * @brief global var for setting zlog
 *****************************************************************************/
extern int server_logging_enabled;
extern zlog_category_t *server_log;
#endif
