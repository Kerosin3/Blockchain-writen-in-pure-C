#ifndef TESTS
#define TESTS

#include "../accounts/acc_utils.h"
#include "../misc/misc.h"
#include "../hashing/hashing.h"
#include <stdlib.h>
#include <stdio.h>
#include <sodium.h>

int test_valid_messages();
int test_INvalid_messages();
int test_hash();
int test_hash_merging();
int test_node_creation();

void tests();

#endif 
