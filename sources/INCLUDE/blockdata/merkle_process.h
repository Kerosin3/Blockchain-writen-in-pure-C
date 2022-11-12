#ifndef MERCLEPROCESS
#define MERCLEPROCESS
#include "layering.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief verifies a message base n merkle tree
 *
 * @param EXPONENT n of messages used in merkle tree
 * @param msg_num message to check
 * @param Layers_pointer layer containers
 * @return true if message matches merklee, else false
 */
int merkle_verify_message(unsigned long long EXPONENT, size_t msg_num, layer_hp *Layers_pointer);

#endif
