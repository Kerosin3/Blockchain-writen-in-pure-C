#include "acc_utils.h"
#include <assert.h>
#include <sodium/crypto_sign.h>
#include <string.h>

signed_message_t sign_a_message(unsigned char* msg,size_t len,unsigned char* secret_key){
	assert(len>0);
	signed_message_t sigmsg;

	unsigned char SK[crypto_sign_SECRETKEYBYTES];
	secret_key = memcpy(SK,secret_key,crypto_sign_SECRETKEYBYTES);
	sigmsg.message = calloc(crypto_sign_BYTES+len, sizeof(unsigned char)); // assign space for message with sign

	unsigned long long signed_message_len; // to write len
	crypto_sign(sigmsg.message, &signed_message_len,
            msg, len, SK);
	sigmsg.length = signed_message_len; // assign msg length
	memset(SK,0,crypto_sign_SECRETKEYBYTES);
	return sigmsg;
}

int validate_a_message(signed_message_t sigmsg,unsigned char* pk){
	unsigned char decoded_message[sigmsg.length - 64];
	unsigned long long decoded_message_len;
	
	unsigned char PK[crypto_sign_PUBLICKEYBYTES];
	memcpy(PK,pk,crypto_sign_PUBLICKEYBYTES);

	if (crypto_sign_open(decoded_message, &decoded_message_len,
                     sigmsg.message, sigmsg.length,(unsigned char*) PK) != 0) {
    		printf("incorrect signature!\n");
		memset(PK,0,crypto_sign_PUBLICKEYBYTES);
    		return -1;
	}
	memset(PK,0,crypto_sign_PUBLICKEYBYTES);
	return 1;

}

user_keys create_key_pair(){
	unsigned char pk[crypto_sign_PUBLICKEYBYTES];
	unsigned char sk[crypto_sign_SECRETKEYBYTES];
	crypto_sign_keypair(pk, sk);
	user_keys keys;
	memcpy(keys.sk, sk,crypto_sign_SECRETKEYBYTES);
	memcpy(keys.pk, pk,crypto_sign_PUBLICKEYBYTES);
	return keys;
}



