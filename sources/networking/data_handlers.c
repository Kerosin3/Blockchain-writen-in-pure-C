#include "data_handlers.h"

#define CIRC_BUF_SIZE 512

void clean_circ_buf(circ_buf_t* cbf);

circ_buf_t create_circ_buf(){
	circ_buf_t cbuf;
	cbuf.fill_size = 0;
	cbuf.maxlen = CIRC_BUF_SIZE;
	cbuf.buffer = ring_data_buf;
	return cbuf;
}

/*
signed_message_t* GET_nth_circ_buf(circ_buf_t* cbuf,size_t N){
	size_t cur_last_msg_serial = (cbuf->fill_size)-1;
	printf("here\n");
	if (N > cur_last_msg_serial) {
		do {
			PUSH_msg_circ_buf(cbuf);
		}while ( (N + 1) != cbuf->fill_size); // push while
		return cbuf->buffer[ (cbuf->fill_size-1) ] ;
	} else {
		return cbuf->buffer[N];
	}

}
*/

signed_message_t* GET_nth_circ_buf(circ_buf_t* cbuf,size_t N){
	size_t cur_last_msg_serial = (cbuf->fill_size)-1;
	if (N > cur_last_msg_serial) {
		do {
			PUSH_msg_circ_buf(cbuf);
		}while ( (N + 1) != cbuf->fill_size); // push while
		return cbuf->buffer[ (cbuf->fill_size-1) ] ;
	} else {
	printf("here\n");
		return cbuf->buffer[N];
	}

}




int PUSH_msg_circ_buf(circ_buf_t* cbuf ){

    	user_keys uk = create_key_pair();

    	signed_message_t* a_msg_p = ls_get_a_signed_msg(uk); // generate random

	size_t cur = cbuf->fill_size; // get current
		      
     	if (server_logging_enabled) zlog_info(server_log, "adding new element to buf");
	// if max
	if (cur == ((cbuf->maxlen )) ) {
     		if (server_logging_enabled) zlog_info(server_log, "maximum buf reached, setting 0");
		clean_circ_buf(cbuf);
		cbuf->fill_size = 0;
		return 1;
	}


	cbuf->buffer[cur] = a_msg_p; // load data to current

	cbuf->fill_size++;// add fill_size
	return 0;
}

signed_message_t* get_cbuf_head(circ_buf_t* cbuf){
	return (cbuf->buffer[(cbuf->fill_size)-1] );
}

void release_circ_nth_msg(circ_buf_t* cbuf,size_t N){
	destroy_signed_message(cbuf->buffer[N]);
}

void clean_circ_buf(circ_buf_t* cbuf){
	for (size_t i = 0; i<CIRC_BUF_SIZE ; i++) {
		if (cbuf->buffer[i]) destroy_signed_message(cbuf->buffer[i]); // free if exists
	}
}


//int check_CIRC_buf(create_circ_buf* cbuf,)
