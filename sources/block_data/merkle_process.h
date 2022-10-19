#ifndef MERCLEPROCESS
#define MERCLEPROCESS
#include "layering.h"
#include <stdio.h>
#include <stdlib.h>

int merkle_verify_message(unsigned long long EXPONENT, size_t msg_num, layer_hp *Layers_pointer);

#endif
