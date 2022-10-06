#ifndef LAYERING
#define LAYERING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hashing/hashing.h"

// struct for layering
typedef struct {
	hash_point_p* main_pointer;
	size_t 	size;
} layer_hp;


layer_hp* process_s_messages(unsigned long long s_msgN,signed_message_t* star_msg);
layer_hp* create_a_h_layer(unsigned long long* size_d_layer, hash_point_p* start_hpointr);
msg_link get_s_msg_from_L0(layer_hp* L0,size_t n);

void destoroy_a_layer(layer_hp* some_layer);
#endif
