#ifndef TESTS
#define TESTS

#include "acc_utils.h"
#include "blockdata.h"
#include "layering.h"
#include "merkle_process.h"
#include "hashing.h"
#include "../networking/start_server.h"
#include "../puzzle/puzzle.h"
#include "../serdes/serdes.h"
#include "zlog.h"
#include "misc.h"
#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int test_valid_messages();
int test_INvalid_messages();
int test_hash();
int test_hash_merging();
int test_node_creation();
int test_Tnode_creation();

void tests();
void tests2();

#endif
