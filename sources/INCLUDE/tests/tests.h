#ifndef TESTS
#define TESTS

#include "acc_utils.h"
#include "blockdata.h"
#include "hashing.h"
#include "layering.h"
#include "merkle_process.h"
#include "start_server.h"
//#include "../puzzle/puzzle.h"
#include "misc.h"
#include "serdes.h"
#include "zlog.h"
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
