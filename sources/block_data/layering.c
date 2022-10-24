#include "layering.h"
#include <sodium/crypto_generichash.h>

// create a level
layer_hp *create_a_h_layer(unsigned long long *size_d_layer, hash_point_p *start_hpointr)
{
    *(size_d_layer) >>= 1LLU; // devide by 2
    unsigned long long _n = *size_d_layer;
    size_t level = 0;
    while (_n >>= 1)
        level++; // calc level
    if ((*size_d_layer) == 1)
    {
        //        printf("create root node\n");
        layer_hp *a_layer = calloc(1, sizeof(layer_hp));
        a_layer->size = *size_d_layer;
        a_layer->level = level;
        hash_point_p *beg_pointer = calloc(*size_d_layer, sizeof(hash_point_p));
        a_layer->main_pointer = beg_pointer;
        *beg_pointer = create_hpoint_hashG(*start_hpointr, start_hpointr[1]);
        //      printf("root hash:\n");
        //    DumpHex(((*(beg_pointer))->hash), crypto_generichash_BYTES);
        //	printf("side hash 1:\n");
        //	DumpHex(  (((hash_point_p) ((*(beg_pointer))->hpoint1)))  , crypto_generichash_BYTES);
        //	printf("side hash 2:\n");
        //	DumpHex(  (((hash_point_p) ((*(beg_pointer))->hpoint2)))  , crypto_generichash_BYTES);
        //	printf("root P is %p\n",a_layer->main_pointer);
        return a_layer;
    }
    // printf("creating n nodes HASH:%llu\n", *size_d_layer);
    layer_hp *a_layer = calloc(1, sizeof(layer_hp));
    a_layer->size = *size_d_layer;
    hash_point_p *beg_pointer = calloc(*size_d_layer, sizeof(hash_point_p));
    a_layer->main_pointer = beg_pointer;
    size_t k = 0;
    for (size_t i = 0; i < ((*size_d_layer) * 2LU); i += 2)
    {
        beg_pointer[k] = create_hpoint_hashG(start_hpointr[i], start_hpointr[i + 1]);
        k++;
    }
    return a_layer;
}

void fill_intermediate_levels(unsigned long long MSG_expt, unsigned long long *n_msg, layer_hp **L_arrays,
                              layer_hp *L_arrays_p)
{

    for (signed k = MSG_expt - 2; k >= 0; --k)
    {
        // printf("k is %d \n", k);
        L_arrays[k] = create_a_h_layer(n_msg, L_arrays[k + 1]->main_pointer);
        L_arrays_p[k] = *L_arrays[k];
        // printf("done!\n");
    }
}

layer_hp *create_LEVEL0(unsigned long long *n_msg, user_keys uk)
{
    signed_message_t *msg_arr[*n_msg];  // arr size /2
    signed_message_t msg_arr_p[*n_msg]; // array for pointers
    for (size_t i = 0; i < *n_msg; i++)
    {
        msg_arr[i] = ls_get_a_signed_msg(uk); // generate random
        validate_a_message(*msg_arr[i], uk.pk);
        msg_arr_p[i] = *msg_arr[i];
        //	DumpHex(msg_arr[i].message, msg_arr[i].length);
    }
    layer_hp *msg_layer = process_s_messages(*n_msg, msg_arr_p); // messages
    (*n_msg) >>= 1LLU;                                           // MESSAGES  LAYER DONE!
    return msg_layer;
}

hashes_hashNode get_a_hashes_Hnode(layer_hp **a_layer, size_t N)
{
    hashes_hashNode a_hash_data;
    a_hash_data.Shash = (*(a_layer[N]->main_pointer))->hash;
    a_hash_data.hash1 = (*(a_layer[N]->main_pointer))->hpoint1;
    a_hash_data.hash2 = (*(a_layer[N]->main_pointer))->hpoint2;
    return a_hash_data;
}

// processing array of messages to a layer 0
layer_hp *process_s_messages(unsigned long long s_msgN, signed_message_t *star_msg)
{
    s_msgN >>= 1; // devide by 2
                  //    printf("msg merged nodes %llu\n", s_msgN);
    layer_hp *a_layer = calloc(1, sizeof(layer_hp));
    a_layer->size = s_msgN; // assign size
    // create storage for porinters
    hash_point_p *beg_pointer = calloc(s_msgN, sizeof(hash_point_p));
    a_layer->main_pointer = beg_pointer;
    for (size_t i = 0; i < (s_msgN); i++)
    {
        // beg_pointer = (void*) s_msgN;
        // beg_pointer[i] = create_hpoint_message(star_msg+i, (star_msg+s_msgN+i ) ); // 0-512 1-513		//WRONG
    }
    return a_layer;
}

// processing array of messages to a layer 0
layer_hp *process_s_messagesV2(unsigned long long s_msgN, signed_message_t **star_msg)
{
    s_msgN >>= 1; // devide by 2
    unsigned long long _n = s_msgN;
    size_t level = 0;
    layer_hp *a_layer = calloc(1, sizeof(layer_hp));
    a_layer->size = s_msgN; // assign size
    while (_n >>= 1)
        level++;            // calc level
    a_layer->level = level; // assign level
                            //   printf("msg merged nodes %llu, layer is %zu\n", s_msgN, level);
    hash_point_p *beg_pointer = calloc(s_msgN, sizeof(hash_point_p));
    a_layer->main_pointer = beg_pointer;
    size_t k = 0;
    for (size_t i = 0; i < (s_msgN * 2LU); i += 2)
    {
        beg_pointer[k] = create_hpoint_message(star_msg[i], (star_msg[i + 1])); // 0-1 2-3 4-5 6-7
        k++;
    }
    return a_layer;
}
/*
void calc_merkle_tree(){
    //-----create basic structures
    unsigned long long EXPONENT = 9;
    unsigned long long n_msg = (1LLU << EXPONENT); //  create 2^9 messages
    layer_hp *L_arrays[EXPONENT];
    layer_hp L_arrays_p[EXPONENT]; // for free
    printf("n msg :%llu\n",n_msg);

    //user_keys uk = create_key_pair();
    signed_message_t** msg_arr = calloc(n_msg, sizeof(signed_message_t *)); //space for msgs
    //----fill messages
    size_t i = 0;
    for (i = 0; i < n_msg; i++)
    {
        msg_arr[i] = ls_get_a_signed_msg(uk); // pointer to message
        validate_a_message(*msg_arr[i], uk.pk);
    }
    //-----------------------------------
    // CREATE BASE LAYER
    L_arrays[EXPONENT-1] = process_s_messagesV2(n_msg, msg_arr);
    L_arrays_p[EXPONENT - 1] = *L_arrays[EXPONENT - 1]; // store pointer
    n_msg >>= 1; // devide by 2
    printf("N OF LEVEL 0 HASH NODES %llu\n", n_msg);
    //--------------------------
    // create intermideate layers
    printf("filling intermideate layers\n");
        fill_intermediate_levels(EXPONENT, &n_msg, L_arrays, L_arrays_p); // done

    //
    // free rootlevel
    for (size_t i = 0; i < EXPONENT; i++)
    {
        destoroy_a_layer(L_arrays[i]);
    }
//     destoroy_a_layer(&L_arrays_p[EXPONENT-1]); // destroy level
    for (i = 0; i < (1LLU<<EXPONENT); i++)
    {
        destroy_signed_message(msg_arr[i]);
    }
    free(msg_arr); // free conrainer for messages


}
*/

// supply n of messages
layer_hp *create_BASE_LAYER(unsigned long long *n_msg, user_keys uk, signed_message_t **first_msg)
{
    signed_message_t *msg_arr[*n_msg];  // create arr size of /2
    signed_message_t msg_arr_p[*n_msg]; // array for pointers
    for (size_t i = 0; i < *n_msg; i++)
    {
        msg_arr[i] = ls_get_a_signed_msg(uk); // generate random
        validate_a_message(*msg_arr[i], uk.pk);
        msg_arr_p[i] = *msg_arr[i];
        //	DumpHex(msg_arr[i].message, msg_arr[i].length);
    }
    layer_hp *msg_layer = process_s_messages(*n_msg, msg_arr_p); // messages
    (*n_msg) >>= 1LLU;                                           // MESSAGES  LAYER DONE!
    return msg_layer;
}

// getting a messages from layer 0
msg_link get_s_msg_from_L0(layer_hp *L0, size_t n)
{
    msg_link a_link;
    if (n >= L0->size)
    {
        a_link.msg1 = 0;
        a_link.msg2 = 0;

        return a_link; // not exists return 0
    }
    a_link.msg1 = ((L0->main_pointer)[n])->messages.smsg_p1;
    a_link.msg2 = ((L0->main_pointer)[n])->messages.smsg_p2;
    return a_link;
}
// allpied for all levels
void destoroy_a_layer(layer_hp *some_layer)
{
    for (size_t i = 0; i < some_layer->size; i++)
    {
        free((some_layer->main_pointer)[i]); // free HPOINTERS
    }
    free(some_layer->main_pointer); // free main pointer
    free(some_layer);               // free layer
}
