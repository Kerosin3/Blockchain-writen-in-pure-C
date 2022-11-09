#ifndef ACCUTILS
#define ACCUTILS
//#include "../misc/misc.h"
#include <sodium.h>
#include <sodium/crypto_sign.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

typedef struct
{
    unsigned char pk[crypto_sign_PUBLICKEYBYTES];
    unsigned char sk[crypto_sign_SECRETKEYBYTES];
} user_keys;

typedef struct
{
    unsigned char *message;
    unsigned long long length;
    unsigned char public_key[crypto_sign_PUBLICKEYBYTES];
} signed_message_t;

typedef struct
{
    unsigned char *message;
    unsigned long long length;
    int ifvalid;

} unsigned_message_t;

typedef struct
{
    unsigned char *test_msg;
    unsigned long long len;
} test_msg_t;
user_keys create_key_pair();

/**
 * @brief sign a message
 *
 * @param testmsg a message to sign
 * @param secret_key private, i.e secret key
 */
signed_message_t sign_a_message(test_msg_t testmsg, unsigned char *secret_key);

/**
 * @brief destuctor for a message
 *
 */
void destroy_signed_message(signed_message_t *a_msg);

/**
 * @brief encorporates a public key to a given message
 *
 */
void put_a_PK(signed_message_t *a_msg, unsigned char *PK);
int validate_a_message(signed_message_t, unsigned char *pk);

#endif
