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



typedef struct {
	signed_message_t* smsg_p1;
	unsigned char hashMSG1[crypto_generichash_BYTES];
	signed_message_t* smsg_p2;
	unsigned char hashMSG2[crypto_generichash_BYTES];
} smgs2;




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

typedef struct {
	hash_point_p* main_pointer;
	size_t 	size;
} layer_hp;

typedef s_link *s_link_p;


typedef struct {
	hash_point_p hpoint1;	
	hash_point_p hpoint2;	
} hash2_point;

typedef hash2_point *hash2_point_p;


typedef struct {
	s_link_p leaf1;
	s_link_p leaf2;
	unsigned char hash[crypto_generichash_BYTES];
} Tnode;

typedef Tnode *Tnode_p;
layer_hp* process_s_messages(unsigned long long s_msgN,signed_message_t* star_msg);
void destoroy_a_layer(layer_hp* some_layer);

hash_point_p create_hpoint_message(signed_message_t* s_msg1, signed_message_t* s_msg2);
hash_point_p create_hpoint_hashG(hash_point_p hp1, hash_point_p hp2);
//hash_point_p create_hpoint_hashL1(hash_point_p hp1, hash_point_p hp2);
//Tnode_p create_node_dtable_LEVEL2(s_link_p node1 ,s_link_p node2);
//s_link_p create_node_dtable(signed_message_t s_messageN1,signed_message_t s_messageN2);
unsigned char* merge_2hashses(unsigned char* hash1, unsigned char* hash2);
unsigned char* calc_hash(signed_message_t s_message);

#endif
