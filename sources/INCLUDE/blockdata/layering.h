#ifndef LAYERING
#define LAYERING

#include "../hashing/hashing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct for layering
typedef struct
{
    hash_point_p *main_pointer;
    size_t size;
    size_t level;
} layer_hp;

typedef struct
{
    unsigned char *Shash;
    unsigned char *hash1;
    unsigned char *hash2;
} hashes_hashNode;

layer_hp *process_s_messagesV2(unsigned long long s_msgN, signed_message_t **star_msg);
hashes_hashNode get_a_hashes_Hnode(layer_hp **a_layer, size_t N);
layer_hp *process_s_messages(unsigned long long s_msgN, signed_message_t *star_msg);
layer_hp *create_a_h_layer(unsigned long long *size_d_layer, hash_point_p *start_hpointr);
msg_link get_s_msg_from_L0(layer_hp *L0, size_t n);
layer_hp *create_LEVEL0(unsigned long long *N_msg, user_keys uk);
void fill_intermediate_levels(unsigned long long MSG_expt, unsigned long long *n_msg, layer_hp **L_arrays,
                              layer_hp *L_arrays_p);

void destoroy_a_layer(layer_hp *some_layer);
#endif
