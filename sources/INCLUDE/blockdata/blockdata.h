#ifndef BLOCKDATA
#define BLOCKDATA

#include "misc.h"
#include <sodium.h>
#include <sodium/crypto_generichash.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NONCE_LEN 15

typedef struct block
{
    unsigned char nonce[NONCE_LEN];
    char date[35];
    uint64_t timestamp_begin;
    uint64_t timestamp_end;
    size_t difficulty;
    void *previous_block;
    unsigned char merkle_root[crypto_generichash_BYTES];

} block_t;
/*
block_t* try_create_block(
        block_t* prev_block,
            unsigned char* merkle_root
        );
*/

void solve_puzzle(unsigned char *merkle_root);
#endif
