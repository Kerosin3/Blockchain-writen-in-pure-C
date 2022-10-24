#ifndef MISC
#define MISC
#include "setup.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
void DumpHex(const void *data, size_t size);

// static void die(const char *message);

#endif
