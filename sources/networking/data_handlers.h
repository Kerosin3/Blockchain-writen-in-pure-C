#ifndef DATA_HANDLE

#define DATA_HANDLE



#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdatomic.h>
#include "../accounts/acc_utils.h"
#include "../misc/misc.h"

typedef struct {
	signed_message_t** buffer;
	size_t head;
	//int tail;
	unsigned long maxlen;
} circ_buf_t;

circ_buf_t create_circ_buf();
void release_circ_nth_msg(circ_buf_t* cbuf,size_t N);
void clean_circ_buf(circ_buf_t* cbuf);

int PUSH_msg_circ_buf(circ_buf_t* cbuf );
size_t get_cbuf_head(circ_buf_t* cbuf);

#endif
