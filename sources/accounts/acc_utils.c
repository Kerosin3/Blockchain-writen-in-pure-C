#include "acc_utils.h"

typedef struct {
	unsigned char pk[crypto_sign_PUBLICKEYBYTES]; 
	unsigned char sk[crypto_sign_SECRETKEYBYTES]; 
} user_keys;

user_keys create_key_pair();
unsigned char* sign_a_message(unsigned char* msg,size_t len,unsigned char* secret_key);
int validate_a_message(unsigned char* signed_message,size_t signed_message_len,unsigned char* pk);


unsigned char* sign_a_message(unsigned char* msg,size_t len,unsigned char* secret_key){
	unsigned char *signed_message = calloc(crypto_sign_BYTES+len, sizeof(char));
	unsigned long long signed_message_len;
	crypto_sign(signed_message, &signed_message_len,
            msg, len, secret_key);
	printf("message signed!\n");
	return signed_message;
}

int validate_a_message(unsigned char* signed_message,size_t signed_message_len,unsigned char* pk){
	unsigned char unsigned_message[signed_message_len];
	unsigned long long unsigned_message_len;
	if (crypto_sign_open(unsigned_message, &unsigned_message_len,
                     signed_message, signed_message_len,(unsigned char*) "dasdasda") != 0) {
    		/* incorrect signature! */
    		printf("incorrect signature!\n");
    		return -1;
	}
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



