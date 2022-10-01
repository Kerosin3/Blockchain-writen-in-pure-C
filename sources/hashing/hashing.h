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

typedef struct {
	signed_message_t s_msg;
	unsigned char msg_hash[crypto_generichash_BYTES];
} link_msg;

typedef struct {
        link_msg  link1;
	link_msg  link2;
	unsigned char hash[crypto_generichash_BYTES];
} s_link;

typedef s_link *s_link_p;

s_link_p create_node_dtable(signed_message_t s_messageN1,signed_message_t s_messageN2);
unsigned char* merge_2hashses(unsigned char* hash1, unsigned char* hash2);
unsigned char* calc_hash(signed_message_t s_message);

#endif
