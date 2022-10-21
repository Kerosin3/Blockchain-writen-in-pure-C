#ifndef MISC
#define MISC

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include "acc_utils.h"

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

signed_message_t* ls_get_a_signed_msg(user_keys keys);


u_int64_t  get_epoch_ns();
long long get_date_usec_rec();
size_t get_timestamp(void* buffer);
test_msg_t ls_get_test_msg(size_t len);
test_msg_t get_test_msg(size_t len);
void DumpHex(const void *data, size_t size);
signed_message_t* get_a_signed_msg(user_keys keys);

#endif
