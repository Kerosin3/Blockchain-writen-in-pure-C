#include "block_process.h"

l_msg_container *calc_merkle_tree(unsigned long long EXPONENT, signed_message_t *msg_pointer)
{
    //-----create basic structures
    l_msg_container *g_cont = calloc(1, sizeof(l_msg_container));
    unsigned long long n_msg = (1LLU << EXPONENT); //  create 2^9 messages
    layer_hp **L_arrays = calloc(EXPONENT, sizeof(layer_hp));
    layer_hp *L_arrays_p = calloc(EXPONENT, sizeof(layer_hp));
    zlog_info(client_log, "now precessing number of messages:",n_msg);

    signed_message_t **msg_arr = calloc(n_msg, sizeof(signed_message_t *));
    //----fill messages
    size_t i = 0;
    g_cont->messages_arr = msg_arr;
    g_cont->main_layer_pointer = L_arrays_p;
    g_cont->layers_for_destr = L_arrays;
    for (i = 0; i < n_msg; i++)
    {
        msg_arr[i] = msg_pointer + i; // pointer to message
    }
    //-----------------------------------
    // CREATE BASE LAYER
    L_arrays[EXPONENT - 1] = process_s_messagesV2(n_msg, msg_arr);
    L_arrays_p[EXPONENT - 1] = *L_arrays[EXPONENT - 1];               // store pointer
    n_msg >>= 1;                                                      // devide by 2
                                                                      //--------------------------
    fill_intermediate_levels(EXPONENT, &n_msg, L_arrays, L_arrays_p); // done

    return g_cont;
}
