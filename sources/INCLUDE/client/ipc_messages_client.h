#ifndef IPCMESSAGESCLIENT
#define IPCMESSAGESCLIENT

#include "acc_utils.h"
#include "transaction.pb-c.h"
#include "blockdata.h"
#include "misc.h"
#include "zlog.h"
#include <liburing.h>
#include <linux/io_uring.h>
#include <netinet/in.h>
#include <sodium/crypto_sign.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "p2ptransaction.pb-c.h"

extern IpcMessage *buffer_transactions;

extern int client_logging_enabled;
extern zlog_category_t *client_log;

extern P2pIpcMessage__Status P2P_deserialize_STATUS(char *buff, unsigned len);

void deserialize_data_from_server(char *buff, unsigned len, signed_message_t *msg);

extern size_t P2P_serialize_block_to_sock(block_t* a_block,void* buf_out);
extern size_t P2P_send_status(P2pIpcMessage__Status STATUS,void* buf_out);
// long long get_date_usec_rec();
IpcMessage__Status read_response_ONLY_STATUS(void *buf, size_t len);
size_t get_a_message(void *buf, size_t len, signed_message_t *a_msg);
size_t send_ONLY_status_code(IpcMessage *message, void *socket_buf, IpcMessage__Status STATUS);

size_t send_need_more_msg(struct io_uring *ring, int sock, void *buffer_wr);

size_t send_ACKN_OK(struct io_uring *ring, int sock, void *buffer_wr);

size_t send_STATUS(struct io_uring *ring, int sock, void *buffer_wr, IpcMessage__Status status_msg);

// void DumpHex(const void *data, size_t size) ;

#endif
