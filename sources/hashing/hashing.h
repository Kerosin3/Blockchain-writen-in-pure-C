#ifndef HASHING

#define HASHING

#include "../accounts/acc_utils.h"
#include "../misc/misc.h"
#include <sodium/crypto_generichash.h>
#include <sodium/crypto_stream_salsa2012.h>
#include <stdlib.h>
#include <stdio.h>
#include <sodium.h>

#define HASHSIZE 32U


//struct for L0 2 messages get_s_msg_from_L0 func
typedef struct {
        signed_message_t*  msg1;
	signed_message_t*  msg2;
} msg_link;

typedef struct {
	signed_message_t* smsg_p1;
	unsigned char hashMSG1[crypto_generichash_BYTES];
	signed_message_t* smsg_p2;
	unsigned char hashMSG2[crypto_generichash_BYTES];
} smgs2;


// generic struct for storing hash_point
typedef struct hash_point_t{
	unsigned char hash[crypto_generichash_BYTES];
	union {
		smgs2 messages;
		struct {
		void* hpoint1; // hash_point_t
		void* hpoint2;
		};
	}; 
	unsigned long long FLAG0;
} hash_point;
typedef hash_point *hash_point_p;

void destroy_hpoint_message(hash_point_p hpoint);

hash_point_p create_hpoint_message(signed_message_t* s_msg1, signed_message_t* s_msg2);
hash_point_p create_hpoint_hashG(hash_point_p hp1, hash_point_p hp2);
unsigned char* merge_2hashses(unsigned char* hash1, unsigned char* hash2);
unsigned char* calc_hash(signed_message_t s_message);

#endif
