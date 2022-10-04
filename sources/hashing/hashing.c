#include "hashing.h"
#include <sodium/crypto_generichash.h>
#include <sodium/crypto_generichash_blake2b.h>
#include <sodium/crypto_hash.h>
#include <sodium/crypto_sign_ed25519.h>
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

/*
 * creates a Tnode with links to level 1 nodes
 *
 */ 
Tnode_p create_node_dtable_LEVEL2(s_link_p node1 ,s_link_p node2){
	// calc sum hash
	unsigned char* shash_p = merge_2hashses(node1->hash,node2->hash  );
	Tnode_p tnode_ints_t = calloc(1,sizeof(Tnode));
	//copy hash
	memcpy(tnode_ints_t->hash,shash_p, crypto_generichash_BYTES);
	tnode_ints_t->leaf1 = node1;	
	tnode_ints_t->leaf2 = node2;	
	free(shash_p);
	return tnode_ints_t;
}
// add verify!!!!!!!!!!!
hash_point_p create_hpoint_message(signed_message_t* s_msg1, signed_message_t* s_msg2){
	hash_point_p hpoint = calloc(1,sizeof(hash_point));
	hpoint->messages.smsg_p1 = s_msg1; // assign 1 msg
	hpoint->messages.smsg_p2 = s_msg2; // assign 2 msg
 
	
	unsigned char* hash_msg1 = calc_hash(*s_msg1);
	memcpy(hpoint->messages.hashMSG1,hash_msg1,crypto_generichash_BYTES);

	unsigned char* hash_msg2 = calc_hash(*s_msg2);
	memcpy(hpoint->messages.hashMSG2  ,hash_msg2,crypto_generichash_BYTES);

	//assign S hash
	unsigned char* shash_p = merge_2hashses(hpoint->messages.hashMSG1, hpoint->messages.hashMSG2  );

	memcpy(hpoint->hash,shash_p,crypto_generichash_BYTES);
	hpoint->FLAG0 = 1;
	free(hash_msg1);
	free(hash_msg2);
	free(shash_p);
	return hpoint;
}

layer_hp* create_a_h_layer(unsigned long long* size_d_layer, hash_point_p start_hpointr){

}

layer_hp* process_s_messages(unsigned long long s_msgN,signed_message_t* star_msg){
	s_msgN>>=1; // devide by 2
	layer_hp* a_layer = calloc(1,sizeof(layer_hp));
	a_layer->size = s_msgN; // assign size
	// create storage for porinters
	
	hash_point_p* beg_pointer = calloc(s_msgN,sizeof(hash_point_p));	
	a_layer->main_pointer = beg_pointer;
	for (size_t i =0; i < (s_msgN); i++) {
		//beg_pointer = (void*) s_msgN;
		beg_pointer[i] = create_hpoint_message(star_msg+i, (star_msg+s_msgN+i ) ); // 0-512 1-513		
	}
	return a_layer;
}

void destoroy_a_layer(layer_hp* some_layer){
	for (size_t i = 0; i< some_layer->size ; i++) {
		free( (some_layer->main_pointer)[i] );
	}
	free(some_layer);
}

/*
// N - n messages
hash_point_p create_hpoint_hashG_rec(unsigned long long N,hash_point_p hp1, hash_point_p hp2){
	N >>= 1; 
	if (N == 1){ // finish
		return 0;
	}

	// calc S hash
	unsigned char* Shash = merge_2hashses(hp1->hash ,hp2->hash );		;
	hash_point_p hpoint = calloc(1,sizeof(hash_point));
	//copy hash
	memcpy(hpoint->hash,Shash,crypto_generichash_BYTES);
	free(Shash);
	// assign pointers to word
	hpoint->hpoint1 = hp1 ;
	hpoint->hpoint2 = hp2;

	hpoint->FLAG0 = 0;
	
	return ( create_hpoint_hashG_rec(N,hp1+ ,hp2+ ) );

}
*/
hash_point_p create_hpoint_hashG(hash_point_p hp1, hash_point_p hp2){
	// calc S hash
	unsigned char* Shash = merge_2hashses(hp1->hash ,hp2->hash );		;
	hash_point_p hpoint = calloc(1,sizeof(hash_point));
	//copy hash
	memcpy(hpoint->hash,Shash,crypto_generichash_BYTES);
	free(Shash);
	// assign pointers to word
	hpoint->hpoint1 = hp1 ;
	hpoint->hpoint2 = hp2;

	hpoint->FLAG0 = 0;
	return hpoint;
}


s_link_p create_node_dtable(signed_message_t s_messageN1,signed_message_t s_messageN2){

	// test messages
	int valid = 0;
	if (!(validate_a_message(s_messageN1,s_messageN1.public_key))){
		//destroy_signed_message(s_messageN1);
		return 0;
	}
	if (!(validate_a_message(s_messageN2,s_messageN2.public_key))){
		//destroy_signed_message(s_messageN2);
		return 0;
	}

	s_link_p  s_link_temp = calloc(1,sizeof(s_link)); // create space for s_link	
	// copy a message stucture
	
	memcpy(&(s_link_temp->link1.s_msg),&s_messageN1,sizeof(signed_message_t));
	memcpy(&(s_link_temp->link2.s_msg),&s_messageN2,sizeof(signed_message_t));
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
//	free(s_messageN1.message); NOT!
//	free(s_messageN2.message);
	return s_link_temp;
}


/*
 * merging two hashes into one, need cleaningup
 */
unsigned char* merge_2hashses(unsigned char* hash1, unsigned char* hash2){
	unsigned char hash1tmp[crypto_generichash_BYTES];
	unsigned char hash2tmp[crypto_generichash_BYTES];
	unsigned char hashS[crypto_generichash_BYTES*2];
	//concat hashes
	memcpy(hash1tmp,hash1, crypto_generichash_BYTES);
	memcpy(hash2tmp,hash2, crypto_generichash_BYTES);
	memcpy(hashS,hash1, crypto_generichash_BYTES);

	memcpy(hashS+crypto_generichash_BYTES,hash2, crypto_generichash_BYTES);
	//calc hashes
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


