#include "data_handlers.h"

#define CIRC_BUF_SIZE 512

void clean_circ_buf(circ_buf_t* cbuf);

circ_buf_t create_circ_buf(){
	circ_buf_t cbuf;
	cbuf.head =0;
	cbuf.maxlen = CIRC_BUF_SIZE;
	cbuf.buffer = 0;
}

int PUSH_msg_circ_buf(circ_buf_t* cbuf ){

    	user_keys uk = create_key_pair();

    	signed_message_t* a_msg_p = ls_get_a_signed_msg(uk); // generate random

	int next;

	next = cbuf->head++; // assign next

	//if (next == cbuf->maxlen ) next = 0;
	if (next == cbuf->maxlen ) {
		printf("MAX BUF REACHED\n");
		clean_circ_buf(cbuf);
		return 1;
	}


	cbuf->buffer[cbuf->head] = a_msg_p; // load data
	cbuf->head = next;
	return 0;
}

size_t get_cbuf_head(circ_buf_t* cbuf){
	return (cbuf->head);
}

void clean_circ_buf(circ_buf_t* cbuf){
	for (size_t i = 0; i<CIRC_BUF_SIZE ; i++) {
		if (cbuf->buffer[i]) destroy_signed_message(cbuf->buffer[i]); // free if exists
	}
}


//int check_CIRC_buf(create_circ_buf* cbuf,)
