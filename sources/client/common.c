#include <common.h>


mtx_t peer_connection_accepted;
atomic_bool kill_thread_p2p;
mtx_t block_created_mtx;
int flag_block_created;
