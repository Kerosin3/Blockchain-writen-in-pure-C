#ifndef IPCMESSAGES

#define IPCMESSAGES

#include "misc.h"
#include "start_server.h"
#include "transaction.pb-c.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief sends status coed to client
 *
 * @param message message to send
 * @param socket_buf client socket
 * @param STATUS status messages (uses serdes messages)
 * @return number of sended  bytes
 */
size_t send_ONLY_status_code(IpcMessage *message, void *socket_buf, IpcMessage__Status STATUS);

/**
 * @brief reads status of client response
 *
 * @param buf buffer to read
 * @param len len of the buffer
 * @return return messages in deserialised form
 */
IpcMessage__Status read_ONLY_STATUS_response(void *buf, size_t len);
#endif
