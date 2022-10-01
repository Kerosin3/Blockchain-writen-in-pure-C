#include "hashing.h"
#include <sodium/crypto_generichash.h>
#include <sodium/crypto_generichash_blake2b.h>
#include <stdlib.h>
#include <string.h>
/*
 *calculating hash for a SIGNED message, need cleaningup, using BLAKE2b algorithm
 */
unsigned char* calc_hash(signed_message_t s_message){
	unsigned char* out_hash = calloc(crypto_generichash_BYTES,sizeof(unsigned char));
	crypto_generichash(out_hash,crypto_generichash_BYTES,
			s_message.message, s_message.length,
			NULL,0);
	return out_hash;
}

s_link_p create_node_dtable(signed_message_t s_messageN1,signed_message_t s_messageN2){

	// test messages
	int valid = 0;
	if (!(validate_a_message(s_messageN1,s_messageN1.public_key))){
		return 0;
	}
	if (!(validate_a_message(s_messageN2,s_messageN2.public_key))){
		return 0;
	}

	s_link_p  s_link_temp = calloc(1,sizeof(s_link)); // create space for s_link	
	// assign a message stucture
	
	memcpy(&s_link_temp->link1.s_msg,&s_messageN1,sizeof(signed_message_t));
	memcpy(&s_link_temp->link2.s_msg,&s_messageN2,sizeof(signed_message_t));
	// assign a hash
	unsigned char* hashmsg1 =  calc_hash((s_link_temp->link1.s_msg));
	memcpy(&s_link_temp->link1.msg_hash,hashmsg1,crypto_generichash_BYTES);
	unsigned char* hashmsg2 =  calc_hash((s_link_temp->link2.s_msg));
	memcpy(&s_link_temp->link2.msg_hash,hashmsg2 ,crypto_generichash_BYTES);
	// calc hash of hashes
	unsigned char* shashes = merge_2hashses(s_link_temp->link1.msg_hash,s_link_temp->link2.msg_hash);
	memcpy(s_link_temp->hash,shashes,crypto_generichash_BYTES);
	free(shashes);
	free(hashmsg1);
	free(hashmsg2);
	return s_link_temp;
}


/*
 * merging two hashes into one, need cleaningup
 */
unsigned char* merge_2hashses(unsigned char* hash1, unsigned char* hash2){
	unsigned char hash1tmp[crypto_generichash_BYTES];
	unsigned char hash2tmp[crypto_generichash_BYTES];
	unsigned char hashS[crypto_generichash_BYTES*2];
	memcpy(hash1tmp,hash1, crypto_generichash_BYTES);
	memcpy(hash2tmp,hash2, crypto_generichash_BYTES);
	memcpy(hashS,hash1, crypto_generichash_BYTES);
	memcpy(hashS+crypto_generichash_BYTES,hash2, crypto_generichash_BYTES);
	unsigned char* out_hash = calloc(crypto_generichash_BYTES,sizeof(unsigned char));
	crypto_generichash(out_hash,crypto_generichash_BYTES,
			hashS, crypto_generichash_BYTES*2,
			NULL,0);
	/*
	printf("\n");
	DumpHex(hash1tmp,crypto_generichash_BYTES);
	printf("\n");
	DumpHex(hash2tmp,crypto_generichash_BYTES);
	printf("\n");
	DumpHex(hashS,crypto_generichash_BYTES*2);
	printf("Shash:\n");
	DumpHex(out_hash,crypto_generichash_BYTES);
	printf("\n");*/
	return out_hash;
}


