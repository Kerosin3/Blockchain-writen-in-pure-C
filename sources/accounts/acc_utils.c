#include "acc_utils.h"
/*
 * return signed message without PK, message need cleanup
 *
 */
signed_message_t sign_a_message(test_msg_t testmsg, unsigned char *secret_key)
{
    ;
    signed_message_t sigmsg;

    unsigned char SK[crypto_sign_SECRETKEYBYTES];
    secret_key = memcpy(SK, secret_key, crypto_sign_SECRETKEYBYTES);
    sigmsg.message =
        calloc(crypto_sign_BYTES + testmsg.len, sizeof(unsigned char)); // assign space for NEW message with sign

    unsigned long long signed_message_len;           // to write len
    crypto_sign(sigmsg.message, &signed_message_len, // copy a messages with signature
                testmsg.test_msg, testmsg.len, SK);
    sigmsg.length = signed_message_len;        // assign msg length
    memset(SK, 0, crypto_sign_SECRETKEYBYTES); // set SK 0
    free(testmsg.test_msg);                    // free test message
    return sigmsg;
}

void destroy_signed_message(signed_message_t *a_msg)
{
    free(a_msg->message);
    free(a_msg);
}

void put_a_PK(signed_message_t *a_msg, unsigned char *PK)
{
    memcpy(a_msg->public_key, PK, crypto_sign_PUBLICKEYBYTES);
}

// 1 - OK
int validate_a_message(signed_message_t sigmsg, unsigned char *pk)
{
    unsigned char decoded_message[sigmsg.length - 64];
    unsigned long long decoded_message_len;

    unsigned char PK[crypto_sign_PUBLICKEYBYTES];
    memcpy(PK, pk, crypto_sign_PUBLICKEYBYTES);

    if (crypto_sign_open(decoded_message, &decoded_message_len, sigmsg.message, sigmsg.length, (unsigned char *)PK) !=
        0)
    {
        printf("incorrect signature!\n");
        memset(PK, 0, crypto_sign_PUBLICKEYBYTES);
        return 0;
    }
    memset(PK, 0, crypto_sign_PUBLICKEYBYTES);
    return 1;
}

user_keys create_key_pair()
{
    unsigned char pk[crypto_sign_PUBLICKEYBYTES];
    unsigned char sk[crypto_sign_SECRETKEYBYTES];
    crypto_sign_keypair(pk, sk);
    user_keys keys;
    memcpy(keys.sk, sk, crypto_sign_SECRETKEYBYTES);
    memcpy(keys.pk, pk, crypto_sign_PUBLICKEYBYTES);
    return keys;
}
