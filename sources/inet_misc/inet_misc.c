#include "inet_misc.h"
#include <sodium/crypto_sign.h>
#include <string.h>

transaction wrap_msg(signed_message_t* a_msg){
	transaction a_transaction;	
	memcpy(a_transaction.public_key,a_msg->public_key,crypto_sign_PUBLICKEYBYTES);
	memcpy(a_transaction.transaction ,a_msg->message ,MSG_SIZE); // FIXED SIZE
	a_transaction.msg_len = htonl(a_msg->length);
	return a_transaction;
}

