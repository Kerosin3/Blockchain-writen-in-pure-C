#ifndef MERCLEPROCESS
#define MERCLEPROCESS
#include <stdlib.h>
#include <stdio.h>
#include "layering.h"

int merkle_verify_message(unsigned long long EXPONENT, size_t msg_num, layer_hp *Layers_pointer);

#endif
