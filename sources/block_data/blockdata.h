#ifndef BLOCKDATA
#define BLOCKDATA

#include <sodium/crypto_generichash.h>
#include <stdio.h>
#include <stdlib.h>
#include <sodium.h>

typedef struct {
	unsigned char nonce[10];
	char date[15];
	unsigned char prev_block[crypto_generichash_BYTES];
	unsigned char merkle_root[crypto_generichash_BYTES];
	
} block;
#endif


