#include "layering.h"
#include <sodium/crypto_generichash.h>



// create a level
layer_hp* create_a_h_layer(unsigned long long* size_d_layer, hash_point_p* start_hpointr){
	*(size_d_layer) >>=1LLU; // devide by 2 

	if ((*size_d_layer) == 1) {
		printf("create root node\n");
		layer_hp* a_layer = calloc(1,sizeof(layer_hp));
		a_layer->size = *size_d_layer;
		hash_point_p* beg_pointer = calloc(*size_d_layer,sizeof(hash_point_p));	
		a_layer->main_pointer = beg_pointer;
		*beg_pointer = create_hpoint_hashG(*start_hpointr, start_hpointr[1] );	
		printf("root hash:\n");
		DumpHex( ((*(beg_pointer))->hash ), crypto_generichash_BYTES);
		printf("side hash 1:\n");
		DumpHex(  (((hash_point_p) ((*(beg_pointer))->hpoint1)))  , crypto_generichash_BYTES);
		printf("side hash 2:\n");
		DumpHex(  (((hash_point_p) ((*(beg_pointer))->hpoint2)))  , crypto_generichash_BYTES);
		printf("root P is %p\n",a_layer->main_pointer);
		return a_layer; 
	}
	printf("creating n nodes HASH:%llu\n",*size_d_layer);
	layer_hp* a_layer = calloc(1,sizeof(layer_hp));
	a_layer->size = *size_d_layer;
	hash_point_p* beg_pointer = calloc(*size_d_layer,sizeof(hash_point_p));	
	a_layer->main_pointer = beg_pointer;
	for (size_t i =0; i < (*size_d_layer); i++) {
		beg_pointer[i] = create_hpoint_hashG(start_hpointr[i], start_hpointr[*size_d_layer+i] );	
	}
	return a_layer;
}


hashes_hashNode get_a_hashes_Hnode(layer_hp** a_layer, size_t N){
	hashes_hashNode a_hash_data;
	a_hash_data.Shash =  (*(a_layer[N]->main_pointer))->hash;
	a_hash_data.hash1 =  (*(a_layer[N]->main_pointer))->hpoint1;
	a_hash_data.hash2 =  (*(a_layer[N]->main_pointer))->hpoint2;
	return a_hash_data;
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
// getting a messages from layer 0 
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
// allpied for all levels
void destoroy_a_layer(layer_hp* some_layer){
	for (size_t i = 0; i< some_layer->size ; i++) {
		free( (some_layer->main_pointer)[i] );
	}
	free(some_layer);
}

