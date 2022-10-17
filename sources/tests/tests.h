#ifndef TESTS
#define TESTS

#include "../accounts/acc_utils.h"
#include "../block_data/blockdata.h"
#include "../block_data/layering.h"
#include "../hashing/hashing.h"
#include "misc.h"
#include "../networking/setup_client.h"
#include "../networking/start_server.h"
#include "../puzzle/puzzle.h"
#include "../serdes/serdes.h"
#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>

int test_valid_messages();
int test_INvalid_messages();
int test_hash();
int test_hash_merging();
int test_node_creation();
int test_Tnode_creation();

void tests();
void tests2();

#endif
