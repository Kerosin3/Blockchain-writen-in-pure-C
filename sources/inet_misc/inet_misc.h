#ifndef INETMISC
#define INETMISC

#include <stdlib.h>
#include <stdio.h>
#include "../accounts/acc_utils.h"

#define MSG_SIZE 100

typedef struct __attribute__((packed)) {
	unsigned char	transaction[MSG_SIZE];
	unsigned long long msg_len;
	unsigned char public_key[crypto_sign_PUBLICKEYBYTES]; 
} transaction;

transaction wrap_msg(signed_message_t* a_msg);

#endif
