#ifndef DATA_HANDLE

#define DATA_HANDLE



#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdatomic.h>
#include "../accounts/acc_utils.h"

typedef struct {
	signed_message_t** buffer;
	size_t head;
	//int tail;
	unsigned long maxlen;
} circ_buf_t;

#endif
