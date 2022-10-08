#ifndef TESTS
#define TESTS

#include "../accounts/acc_utils.h"
#include "../misc/misc.h"
#include "../hashing/hashing.h"
#include "../block_data/blockdata.h"
#include "../block_data/layering.h"
#include "../puzzle/puzzle.h"
#include "../networking/start_server.h"
#include "../networking/setup_client.h"
#include "../serdes/serdes.h"
#include <stdlib.h>
#include <stdio.h>
#include <sodium.h>

int test_valid_messages();
int test_INvalid_messages();
int test_hash();
int test_hash_merging();
int test_node_creation();
int test_Tnode_creation();

void tests();
void tests2();

#endif 
