#include "blockdata.h"
#include <string.h>
/*
block_t* try_create_block(
		block_t* prev_block,
	        unsigned char* merkle_root
		)
{
	block_t* blk = calloc(1,sizeof(block_t));
	//copy merkle root
	memcpy(blk->merkle_root,merkle_root,crypto_generichash_BYTES);
	//set time
	get_timestamp(blk->date); 
	//set beg timestampn
	blk->timestamp_begin = get_epoch_ns();
	blk->timestamp_end = 0; 
	blk->previous_block = 0;
	blk->difficulty = 0;
	memset(blk->nonce,0,NONCE_LEN);
	return blk; 
}
*/

int test_solution(unsigned char* merkle_root, char* nonce,unsigned char* answer){
	unsigned char out_hash[crypto_generichash_BYTES] = {0};
	unsigned char CONCAT_BYTES[NONCE_LEN+crypto_generichash_BYTES] = {0};
	memcpy(CONCAT_BYTES,nonce,NONCE_LEN); //copy nonce
	memcpy(CONCAT_BYTES+NONCE_LEN,merkle_root,crypto_generichash_BYTES);// concat
									    //
	crypto_generichash(out_hash,crypto_generichash_BYTES,
			CONCAT_BYTES, crypto_generichash_BYTES+NONCE_LEN,
			NULL,0); //calc HASH
	printf("hash:\n");
	DumpHex(out_hash,crypto_generichash_BYTES);
	printf("----------\n");
	if ((!memcmp(out_hash,answer,crypto_generichash_BYTES))) {
		printf("MATCH!\n");
		return 1;
	}
	return 0;
}

void solve_puzzle(unsigned char* merkle_root){
    if (sodium_init() < 0 ) die("libsodium!"); 

    unsigned char nonce_answ[NONCE_LEN] = {0};

//     unsigned char* nonce = calloc(NONCE_LEN,sizeof(unsigned char)); //alocate for hash
    unsigned char* CONCAT_BYTES = calloc(NONCE_LEN+crypto_generichash_BYTES ,sizeof(unsigned char)); //allocate concat bytes
    memcpy(CONCAT_BYTES+NONCE_LEN,merkle_root,crypto_generichash_BYTES); // copy merkle to concat
	    
//     unsigned char CONCAT_BYTES[crypto_generichash_BYTES+NONCE_LEN] = {0}; // concat bytes

    unsigned char* out_hash = calloc(crypto_generichash_BYTES,sizeof(unsigned char)); //alocate for hash

    unsigned char ans[5] = {'\0','\0','\0','\0','\0'};// use 3 only
    do {
        randombytes_buf(CONCAT_BYTES, NONCE_LEN); // take random sample to CONCAT
    	crypto_generichash(out_hash,crypto_generichash_BYTES,
		CONCAT_BYTES, crypto_generichash_BYTES+NONCE_LEN,
			NULL,0); //calc HASH

// 	printf("current hash:\n");
    }while (  (memcmp(out_hash,ans,3   ) )  );
    printf("answer:\n");
    DumpHex(out_hash,crypto_generichash_BYTES);
    printf("nonce:\n");
    memcpy(nonce_answ,CONCAT_BYTES,NONCE_LEN);
    DumpHex(nonce_answ,NONCE_LEN);
    free(CONCAT_BYTES);

    test_solution(merkle_root,nonce_answ,out_hash);
    free(out_hash);
}



