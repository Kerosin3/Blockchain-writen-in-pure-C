#ifndef IPCMESSAGES

#define IPCMESSAGES

#include "../serdes/transaction.pb-c.h"
#include "../misc/misc.h"
#include <stdlib.h>
#include <stdio.h>

size_t send_ONLY_status_code( IpcMessage* message,void* socket_buf, IpcMessage__Status STATUS);

IpcMessage__Status read_ONLY_STATUS_response(void* buf,size_t len);
#endif
