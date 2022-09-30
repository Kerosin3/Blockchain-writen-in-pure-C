#ifndef MISC
#define MISC

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

typedef struct {
	unsigned char* test_msg;
	unsigned long long len;
} test_msg_t;

test_msg_t get_test_msg(size_t len);
void DumpHex(const void *data, size_t size);

#endif
