#ifndef INETMISC
#define INETMISC

#include <stdlib.h>
#include <stdio.h>
#include "../accounts/acc_utils.h"
#include <stdint.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define MSG_SIZE 100

typedef struct __attribute__((packed)) {
	u_int8_t transaction[MSG_SIZE];
	u_int32_t msg_len;
	u_int8_t public_key[crypto_sign_PUBLICKEYBYTES]; 
} transaction;

transaction wrap_msg(signed_message_t* a_msg);

#endif
