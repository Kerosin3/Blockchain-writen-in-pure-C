#ifndef SERDES

#define SERDES
#include "transaction.pb-c.h"
#include "acc_utils.h"
#include "misc.h"

#include <sodium/crypto_sign.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
size_t serialize_data_v2(void* socket_buf,signed_message_t* a_message,IpcMessage* msg_buf);
void print_serialized_data(void* buffer_in,size_t len);

signed_message_t deserialize_data(int sock,void* deserialized_data);

#endif
