#ifndef p2p_setup

#define p2p_setup
#include "blockdata.h"
#include "p2p_settings.h"
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
#include <threads.h>
#include <unistd.h>

/** @file p2p_setup.h
 * Setupping p2p io_uring
 */

extern char *buffers;
extern size_t *buffer_lengths;
extern size_t *buffer_sended_N;
/**
 * @brief external buffer for serialized block data
 *
 */
extern unsigned char *buffer_BLOCK_DATA; //!< searealized block assembled from messages
extern void die(char *);

void destroy_buffers();
void setup_buffers();

/*!
 * @brief setting up a server for P2P communications on port 10001
 * @param port port to start on,hardcoded 10001
 *****************************************************************************/
int setup_serv_sock(uint16_t port);
/*!
 * @brief setting up iouring structure, polling disabled
 *****************************************************************************/

/**
 * @brief just teardown the sock after app finished
 *
 * @param servsock sock to shutdown
 */
void teardown_server_sock(int servsock);
void setup_iouring(struct io_uring *ring, bool pooling);

#endif
