#ifndef DATA_HANDLE

#define DATA_HANDLE



//#include "start_server.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdatomic.h>
#include "../accounts/acc_utils.h"
#include "misc.h"
#include "zlog.h"
#include "setup.h"

extern int server_logging_enabled;
extern zlog_category_t *server_log;

/**
 * @brief circular buffer used for...
 * @param buffer signed message pointer
 * @return circular buf obj
**************************************************************************/
typedef struct {
	signed_message_t** buffer;
	size_t fill_size;
	unsigned long maxlen;
} circ_buf_t;

signed_message_t* GET_last_circ_buf(circ_buf_t* cbuf);
circ_buf_t create_circ_buf();
void release_circ_nth_msg(circ_buf_t* cbuf,size_t N);
void clean_circ_buf(circ_buf_t* cbuf);

int PUSH_msg_circ_buf(circ_buf_t* cbuf );

signed_message_t* GET_nth_circ_buf(circ_buf_t* cbuf,size_t N);
signed_message_t* get_cbuf_head(circ_buf_t* cbuf);

#endif
