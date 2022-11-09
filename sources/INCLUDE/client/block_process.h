#ifndef BLOCKPROC
#define BLOCKPROC

#include "blockdata.h"
#include "layering.h"
#include "zlog.h"
#include "hashing.h"

typedef struct
{
    layer_hp *main_layer_pointer;
    layer_hp **layers_for_destr;
    signed_message_t **messages_arr;
} l_msg_container;

extern zlog_category_t *client_log;
extern int client_logging_enabled;
l_msg_container *calc_merkle_tree(unsigned long long, signed_message_t *msg_pointer);

block_t* processing_block(unsigned long long EXPONENT, signed_message_t* msg_arr, l_msg_container* L_arrays_p_cont);
#endif
