#include "block_process.h"




l_msg_container* calc_merkle_tree(unsigned long long EXPONENT, signed_message_t* msg_pointer){
    //-----create basic structures	
    l_msg_container* g_cont = calloc(1,sizeof(l_msg_container));
    unsigned long long n_msg = (1LLU << EXPONENT); //  create 2^9 messages
    // layer_hp* L_arrays[EXPONENT];
    layer_hp** L_arrays = calloc(EXPONENT,sizeof(layer_hp));
//    layer_hp L_arrays_p[EXPONENT]; // for free
    layer_hp* L_arrays_p = calloc(EXPONENT,sizeof(layer_hp));
    printf("n msg :%llu\n",n_msg);

    //user_keys uk = create_key_pair();
    signed_message_t** msg_arr = calloc(n_msg, sizeof(signed_message_t *));
    //----fill messages
    size_t i = 0;
    g_cont->messages_arr = msg_arr;
    g_cont->main_layer_pointer = L_arrays_p;
    g_cont->layers_for_destr = L_arrays;
    for (i = 0; i < n_msg; i++)
    {
        msg_arr[i] = msg_pointer+i; // pointer to message

//  	   DumpHex( msg_pointer[i].message, msg_pointer[i].length );
//         validate_a_message(*msg_arr[i], uk.pk);
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
    printf("calcing puzzle\n");

 // DumpHex( ((*(L_arrays_p[0].main_pointer))->hash ), crypto_generichash_BYTES);
//	solve_puzzle(  ((*(L_arrays_p[0].main_pointer))->hash )  ); //???????????????????
//    L_arrays[0]->main_pointer // 
	//
	// free rootlevel
/*
    
    for (size_t i = 0; i < EXPONENT; i++) // free in called process
    {
        destoroy_a_layer(L_arrays[i]);
    }*/
    /*for (i = 0; i < (1LLU<<EXPONENT); i++) // destroy in calling process
    {
        //destroy_signed_message(msg_arr[i]);
    }*/
    //free(msg_arr); // free conrainer for messages
   return g_cont;

}


