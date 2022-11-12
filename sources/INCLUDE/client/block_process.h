#ifndef BLOCKPROC
#define BLOCKPROC

#include "blockdata.h"
#include "hashing.h"
#include "layering.h"
#include "zlog.h"

/**
 * struct {name} - struct for firs layer merkle tree
 */
typedef struct
{
    layer_hp *main_layer_pointer;
    layer_hp **layers_for_destr;
    signed_message_t **messages_arr;
} l_msg_container;

extern zlog_category_t *client_log;
extern int client_logging_enabled;

/**
 * @brief calculates a merkle tree for a given set of signed messages
 *
 * @param exponent number of messages
 * @param msg_pointer msg container
 * @return returns  layer containers to store merkle layers
 */
l_msg_container *calc_merkle_tree(unsigned long long exponent, signed_message_t *msg_pointer);

/**
 * @brief calculates puzzle for a given hash of a block
 *
 * @param L_arrays_p_cont layers of merkle tree
 * @return pointer block with an assigned hash
 */
block_t *processing_block(l_msg_container *L_arrays_p_cont);
#endif
