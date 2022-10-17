#include "data_handlers.h"

#define CIRC_BUF_SIZE 512

circ_buf_t create_circ_buf(){
	circ_buf_t cbuf;
	cbuf.head =0;
	cbuf.maxlen = CIRC_BUF_SIZE;
	cbuf.buffer = 0;
}

int PUSH_msg_circ_buf(circ_buf_t* cbuf, signed_message_t* msg ){
	int next;

	next = cbuf->head++; // assign next

	if (next == cbuf->maxlen ) next = 0;

//	if (next == cbuf->tail) return -1;

	cbuf->buffer[cbuf->head] = msg; // load data
	cbuf->head = next;
	return 0;
}


//int check_CIRC_buf(create_circ_buf* cbuf,)
