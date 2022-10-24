#include "hashing.h"
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

unsigned char* calc_hashof_hash(unsigned char* hash0){
	unsigned char* out_hash = calloc(crypto_generichash_BYTES,sizeof(unsigned char));
	crypto_generichash(out_hash,crypto_generichash_BYTES,
			hash0, crypto_generichash_BYTES,
			NULL,0);
	return out_hash;
}




// add verify!!!!!!!!!!!
// store pointer to messages!
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
	hpoint->FLAG0 = 0;
	free(hash_msg1); //ok
	free(hash_msg2); //ok
	free(shash_p);   //ok
	hash_msg2 = NULL;
	hash_msg1 = NULL;
	shash_p = NULL;
	return hpoint;
}
void destroy_hpoint_message(hash_point_p hpoint){
	free (hpoint);
}
/*
void create_inermmediate_layers(size_t n_layers,layer_hp* layer_beg){
	
	for (signed k=7; k>=0 ; --k ) { 
		printf("k is %d \n",k);
		if (k==7) {
		layer_beg[k] =* create_a_h_layer(&n_msg,L0pointer);
		continue;
		}
		L_arrays[k] = *create_a_h_layer(&n_msg,L_arrays[k+1].main_pointer );
	}
}
*/

// creare a host node from 2 nodes
hash_point_p create_hpoint_hashG(hash_point_p hp1, hash_point_p hp2){
	// calc S hash
	unsigned char* Shash = merge_2hashses(hp1->hash ,hp2->hash );		;
	hash_point_p hpoint = calloc(1,sizeof(hash_point));
	//copy hash
	memcpy(hpoint->hash,Shash,crypto_generichash_BYTES);
	//printf("\n");
	//DumpHex(hpoint->hash, crypto_generichash_BYTES);
	free(Shash);
	// assign pointers to word
	hpoint->hpoint1 = hp1 ;
	hpoint->hpoint2 = hp2;

	hpoint->FLAG0 = 1;
	return hpoint;
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


