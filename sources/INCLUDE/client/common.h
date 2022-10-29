#ifndef COMMON
#define COMMON

#include <stdlib.h>
#include <stdint.h>
#include <threads.h>
#include <stdatomic.h>

extern mtx_t block_created_mtx;
extern int flag_block_created;
extern unsigned char *buffer_BLOCK_DATA;
extern atomic_bool kill_thread_p2p;


#endif
