#ifndef MISC
#define MISC

#include "acc_utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

signed_message_t *ls_get_a_signed_msg(user_keys keys);

/**
 * @brief return timestamp in epoh format
 *
 * @return epoh format
 */
u_int64_t get_epoch_ns();
long long get_date_usec_rec();
size_t get_timestamp(void *buffer);

/**
 * @brief libsodium-based random message
 *
 * @param len length of a message
 * @return pointer of th message
 */
test_msg_t ls_get_test_msg(size_t len);
/**
 * @brief custom-made ramdom message
 *
 * @param len length of message
 * @return pointer to a message
 */
test_msg_t get_test_msg(size_t len);
void DumpHex(const void *data, size_t size);
signed_message_t *get_a_signed_msg(user_keys keys);

#endif
