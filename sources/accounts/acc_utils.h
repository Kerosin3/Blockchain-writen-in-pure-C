#ifndef ACCUTILS
#define ACCUTILS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sodium.h>
#include <sodium/crypto_sign.h>


typedef struct {
	unsigned char pk[crypto_sign_PUBLICKEYBYTES]; 
	unsigned char sk[crypto_sign_SECRETKEYBYTES]; 
} user_keys;

typedef struct {
	unsigned char* message;
	unsigned long long length;
	unsigned char public_key[crypto_sign_PUBLICKEYBYTES]; 
} signed_message_t;

typedef struct {
	unsigned char* message;
	unsigned long long length;
	int 	ifvalid;

} unsigned_message_t;

user_keys create_key_pair();

void destroy_signed_message(signed_message_t a_msg);
void put_a_PK(signed_message_t* a_msg,unsigned char* PK);
signed_message_t sign_a_message(unsigned char* msg,size_t len,unsigned char* secret_key);
int validate_a_message(signed_message_t,unsigned char* pk);

#endif
