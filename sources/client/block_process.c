#include "block_process.h"

l_msg_container *calc_merkle_tree(unsigned long long EXPONENT, signed_message_t *msg_pointer)
{
    //-----create basic structures
    l_msg_container *g_cont = calloc(1, sizeof(l_msg_container));
    unsigned long long n_msg = (1LLU << EXPONENT); //  create 2^9 messages for fors layer if exponen is 9
    layer_hp **L_arrays = calloc(EXPONENT, sizeof(layer_hp));
    layer_hp *L_arrays_p = calloc(EXPONENT, sizeof(layer_hp));
    zlog_info(client_log, "now processing number of messages: %llu", n_msg);

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
    n_msg >>= 1;                                                      // div by 2
    fill_intermediate_levels(EXPONENT, &n_msg, L_arrays, L_arrays_p); // done

    return g_cont;
}



block_t *processing_block(l_msg_container *L_arrays_p_cont)
{

    DumpHex((*(L_arrays_p_cont->main_layer_pointer[0].main_pointer))->hash, crypto_generichash_BYTES);
    unsigned char *temp_hash =
        calc_hashof_hash((*(L_arrays_p_cont->main_layer_pointer[0].main_pointer))->hash); // calc hash of hash BIRTHDAY
    printf("Birthday hash (hash of hash):\n");
    DumpHex(temp_hash, crypto_generichash_BYTES);
    free(temp_hash);
    unsigned char merkle_root_first[crypto_generichash_BYTES];
    memcpy(merkle_root_first, (*(L_arrays_p_cont->main_layer_pointer[0].main_pointer))->hash, crypto_generichash_BYTES);
    //****************************************************/
    printf("-=STARTING PUZZLE SOLVING WITH DIFFICULTY 3=-\n");
    puzzle_thr_container* pc = calloc(1,sizeof(puzzle_thr_container));

    unsigned char *nonce_answ = calloc(NONCE_LEN, sizeof(unsigned char));
    pc->merkle_root_p = merkle_root_first;
    pc->nonce = nonce_answ;
    thrd_t thread_calc_puzzle;        // create thread listen
    int tc_ret = thrd_create(&thread_calc_puzzle, (thrd_start_t)solve_puzzle_fixed_diff, (void *)pc);
    //------------------------------------------------------------------------------------------------------>>ok
    if (tc_ret == thrd_error)
    {
        printf("error while thread computing solution \n");
        //exit(1);
    }
    int rez=0;
    if (thrd_join(thread_calc_puzzle,&rez) != thrd_success)
    {
        printf("error joining thread \n");
    }
    //thrd_detach(thread_p2p_list); // dETACH HOW?
    printf("thread computing solution has finished!\n");
    //unsigned char *nonce = solve_puzzle(merkle_root_first, 3); // calc puzzle 3 = difficulty 
    printf("-=PUZZLE HAS BEEN SOLVED=-\n");
    // create block
    block_t *block_dummy = create_block_dummy(0, merkle_root_first);
    set_nonce_to_block(block_dummy, nonce_answ);
    set_prev_block_hash(block_dummy, NULL);
    free(pc); // free container
    return block_dummy;
}
