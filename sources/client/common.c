#include <common.h>


mtx_t peer_connection_accepted;
atomic_bool kill_thread_p2p;
atomic_bool block_transmitted;
atomic_uint_least64_t block_written_size;
mtx_t block_created_mtx;
int flag_block_created;
