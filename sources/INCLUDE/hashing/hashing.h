#ifndef HASHING

#define HASHING

#include "acc_utils.h"
#include "misc.h"
#include <sodium.h>
#include <sodium/crypto_generichash.h>
#include <sodium/crypto_stream_salsa2012.h>
#include <stdio.h>
#include <stdlib.h>

#define HASHSIZE 32U

// struct for L0 2 messages get_s_msg_from_L0 func
typedef struct
{
    signed_message_t *msg1;
    signed_message_t *msg2;
} msg_link;

typedef struct
{
    signed_message_t *smsg_p1;
    unsigned char hashMSG1[crypto_generichash_BYTES];
    signed_message_t *smsg_p2;
    unsigned char hashMSG2[crypto_generichash_BYTES];
} smgs2;

unsigned char *calc_hashof_hash(unsigned char *hash0);

// generic struct for storing hash_point
typedef struct hash_point_t
{
    unsigned char hash[crypto_generichash_BYTES];
    union {
        smgs2 messages;
        struct
        {
            void *hpoint1; // hash_point_t
            void *hpoint2;
        };
    };
    unsigned long long FLAG0;
} hash_point;
typedef hash_point *hash_point_p;

void destroy_hpoint_message(hash_point_p hpoint);

/**
 * @brief creates first layer node messages
 *
 * @param s_msg1 message 1
 * @param s_msg2 message 2
 * @return node of first level
 */
hash_point_p create_hpoint_message(signed_message_t *s_msg1, signed_message_t *s_msg2);

/**
 * @brief creates inttermideate layer node in merkle tree
 *
 * @param hp1 int. node 1
 * @param hp2 int. node 2
 * @return int. node
 */
hash_point_p create_hpoint_hashG(hash_point_p hp1, hash_point_p hp2);

/**
 * @brief computes hash of two hashes
 *
 * @param hash1 hash 1
 * @param hash2 hash 2
 */
unsigned char *merge_2hashses(unsigned char *hash1, unsigned char *hash2);
/**
 * @brief calculate hash of a message, need to free memory
 *
 * @param s_message signed message
 */
unsigned char *calc_hash(signed_message_t s_message);

#endif
