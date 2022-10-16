#include "block_process.h"




void calc_merkle_tree(signed_message_t* msg_pointer){
    //-----create basic structures	
    unsigned long long EXPONENT = 9;
    unsigned long long n_msg = (1LLU << EXPONENT); //  create 2^9 messages
    layer_hp *L_arrays[EXPONENT];
    layer_hp L_arrays_p[EXPONENT]; // for free
    printf("n msg :%llu\n",n_msg);

    //user_keys uk = create_key_pair();
    signed_message_t** msg_arr = calloc(n_msg, sizeof(signed_message_t *));
    //----fill messages
    size_t i = 0;
    for (i = 0; i < n_msg; i++)
    {
        msg_arr[i] = msg_pointer+i; // pointer to message

  	   DumpHex( msg_pointer->message, msg_pointer->length  ); 
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

	//
	// free rootlevel
    for (size_t i = 0; i < EXPONENT; i++)
    {
        destoroy_a_layer(L_arrays[i]);
    }
   /* for (i = 0; i < (1LLU<<EXPONENT); i++)
    {
        destroy_signed_message(msg_arr[i]);
    }*/
    free(msg_arr); // free conrainer for messages


}


