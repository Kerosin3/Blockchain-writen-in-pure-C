#include "ipcmessages.h"

size_t send_ONLY_status_code(IpcMessage *message, void *socket_buf, IpcMessage__Status STATUS)
{
    int len = 0;
    char *date[32];
    memset(message, 0, sizeof(IpcMessage)); // clear
    ipc_message__init(message);

    message->has_pubkey = 0;
    message->has_transaction_msg = 0;

    size_t time_len = get_timestamp(date);
    message->timestamp = date; // its ok
    message->time_num = get_epoch_ns();                               //
    message->status_code = STATUS;

    len = ipc_message__get_packed_size(message);
    ipc_message__pack(message, socket_buf); // write to buffer
    return len;
}

IpcMessage__Status read_ONLY_STATUS_response(void *buf, size_t len)
{
    IpcMessage__Status status;
    IpcMessage *message;
    message = ipc_message__unpack(0, len, buf);
    printf("STATUS CODE IS %d\n", message->status_code);
    printf("READED MESSAGE TIMESTAMP:%llu\n",message->time_num);
    //	printf("TIMESTAMP:%s\n",message->timestamp );
    status = message->status_code;
    ipc_message__free_unpacked(message, NULL);
    return status;
}
