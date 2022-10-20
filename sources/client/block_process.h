#ifndef BLOCKPROC
#define BLOCKPROC


#include "../block_data/layering.h"
#include "../block_data/blockdata.h"

typedef struct{
	layer_hp* main_layer_pointer;
	layer_hp** layers_for_destr;
	signed_message_t** messages_arr;
} l_msg_container;

l_msg_container* calc_merkle_tree(unsigned long long,signed_message_t* msg_pointer);

#endif
