#ifndef BLOCKDATA
#define BLOCKDATA

#include "hashing.h"
#include "misc.h"
#include <sodium.h>
#include <sodium/crypto_generichash.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NONCE_LEN 15
// block struct
typedef struct __attribute__((packed)) block
{
    unsigned char nonce[NONCE_LEN];
    char date[35];
    uint64_t timestamp_begin;
    uint64_t timestamp_end;
    size_t difficulty;
    unsigned char previous_block[crypto_generichash_BYTES];
    unsigned char merkle_root[crypto_generichash_BYTES];

} block_t;

void print_block_data(block_t *block);
void set_prev_block_hash(block_t *blk_current, block_t *blk_prev);
/**
 * @brief setting calced nonce to block
 *
 * @param blk block to assign
 * @param nonce nonce to assign
 */
void set_nonce_to_block(block_t *blk, unsigned char *nonce);

/**
 * @brief solving
 *
 * @param merkle_root merkle root to process
 * @param difficulty difficulty: 1-5;
 */
unsigned char *solve_puzzle(unsigned char *merkle_root, unsigned difficulty);
/**
 * @brief creates dimmy for a block
 *
 * @param prev_block prev block, 0 if first block
 * @param merkle_root calced merkle root
 * @return pointer to block dimmy
 */
block_t *create_block_dummy(block_t *prev_block, unsigned char *merkle_root);

#endif
