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
// processing array of messages to a layer 0
layer_hp* process_s_messages(unsigned long long s_msgN,signed_message_t* star_msg){
	s_msgN>>=1; // devide by 2
	printf("msg merged nodes %llu\n",s_msgN);
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

msg_link get_s_msg_from_L0(layer_hp* L0,size_t n){
	msg_link a_link;
	if (n >= L0->size){
		a_link.msg1 = 0;
		a_link.msg2 = 0;

		return a_link; // not exists return 0
	}
	a_link.msg1 = ((L0->main_pointer)[n])->messages.smsg_p1;
	a_link.msg2 = ((L0->main_pointer)[n])->messages.smsg_p2;
	return a_link;
}

void destoroy_a_layer(layer_hp* some_layer){
	for (size_t i = 0; i< some_layer->size ; i++) {
		free( (some_layer->main_pointer)[i] );
	}
	free(some_layer);
}


layer_hp* create_a_h_layer(unsigned long long* size_d_layer, hash_point_p* start_hpointr){
	if ((*size_d_layer) == 1) {
		// end
	}
	*(size_d_layer) >>=1LLU; // devide by 2 
	printf("n nodes HASH:%llu\n",*size_d_layer);
	layer_hp* a_layer = calloc(1,sizeof(layer_hp));
	a_layer->size = *size_d_layer;
	hash_point_p* beg_pointer = calloc(*size_d_layer,sizeof(hash_point_p));	
	a_layer->main_pointer = beg_pointer;
	for (size_t i =0; i < (*size_d_layer); i++) {
		beg_pointer[i] = create_hpoint_hashG(start_hpointr[i], start_hpointr[*size_d_layer+i] );	
	}
	return a_layer;
}


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


