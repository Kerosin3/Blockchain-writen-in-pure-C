#include "blockdata.h"

void die(const char*);


block_t* create_block_dummy(
        block_t* prev_block,
            unsigned char* merkle_root
        )
{    
    block_t* blk = calloc(1,sizeof(block_t));
    //copy merkle root
    unsigned char* temp_hash = calc_hashof_hash(merkle_root);// calc hash of hash BIRTHDAY
    memcpy(blk->merkle_root,temp_hash,crypto_generichash_BYTES); // copy this hash
    free(temp_hash);
    //set time
    get_timestamp(blk->date); // date creation
    //set beg timestampn
    blk->timestamp_begin = get_epoch_ns(); // set timestamp block created
    blk->timestamp_end = 0;  // set when finish puzzle calcing
    if (!prev_block) { // first block -> just zeros
	    memset(blk->previous_block,0,crypto_generichash_BYTES);
    } else {// calc hash o prev block
	unsigned char* out_hash = calloc(crypto_generichash_BYTES,sizeof(unsigned char));
	crypto_generichash(out_hash,crypto_generichash_BYTES,
			(unsigned char*) prev_block,sizeof(block_t),
			NULL,0);
	memcpy(blk->previous_block,out_hash,crypto_generichash_BYTES);// copy hash
	free(out_hash);
    }
    blk->difficulty = 0;
    memset(blk->nonce,0,NONCE_LEN);
    printf("block dummy has been created!\n");
    return blk;
}

void print_block_data(block_t* block ){
	printf("-----------------block content starts-----------------\n");
	printf("block data creation:\n");
	DumpHex(block->date,32);
	printf("block nonce:\n");
	DumpHex(block->nonce,15);
	printf("block merkle root:\n");
	DumpHex(block->merkle_root,32);
	printf("-----------------block content ends-------------------\n");
}

void set_nonce_to_block(block_t* blk,unsigned char* nonce){
    memcpy(blk->nonce,nonce,NONCE_LEN);
    blk->timestamp_end = get_epoch_ns(); // assign end time
    free(nonce);
}

void set_prev_block_hash(block_t* blk_current, block_t* blk_prev){
	if (!blk_prev) {
		memset(blk_current->previous_block,0,crypto_generichash_BYTES);
		return;
	}
        unsigned char* prev_block_hash_of_hash = calc_hashof_hash(blk_prev->merkle_root);// calc hash of hash BIRTHDAY
	memcpy(blk_current->previous_block, prev_block_hash_of_hash,crypto_generichash_BYTES);
	free(prev_block_hash_of_hash);
}

int test_solution(unsigned char *merkle_root, char *nonce, unsigned char *answer)
{
    unsigned char out_hash[crypto_generichash_BYTES] = {0};
    unsigned char CONCAT_BYTES[NONCE_LEN + crypto_generichash_BYTES] = {0};
    memcpy(CONCAT_BYTES, nonce, NONCE_LEN);                                  // copy nonce
    memcpy(CONCAT_BYTES + NONCE_LEN, merkle_root, crypto_generichash_BYTES); // concat
                                                                             //
    crypto_generichash(out_hash, crypto_generichash_BYTES, CONCAT_BYTES, crypto_generichash_BYTES + NONCE_LEN, NULL,
                       0); // calc HASH
    printf("hash of resulted solution:\n");
    printf("---------------------------------------\n");
    DumpHex(out_hash, crypto_generichash_BYTES);
    printf("---------------------------------------\n");
    if ((!memcmp(out_hash, answer, crypto_generichash_BYTES)))
    {
        printf("test passed nonce finished successfully!\n");
        return 1;
    }
    return 0;
}

unsigned char* solve_puzzle(unsigned char *merkle_root,unsigned difficulty)
{
    if (difficulty > 5){
	    difficulty = 5;
	}
    if (difficulty == 0 ) difficulty = 1;

    if (sodium_init() < 0)
        die("libsodium!");
    
    unsigned char* nonce_answ = calloc(NONCE_LEN,sizeof(unsigned char));

    unsigned char *CONCAT_BYTES =
        calloc(NONCE_LEN + crypto_generichash_BYTES, sizeof(unsigned char)); // allocate concat bytes
    memcpy(CONCAT_BYTES + NONCE_LEN, merkle_root, crypto_generichash_BYTES); // copy merkle to concat
    printf("calcing hash puzzle for merkle root: difficulty:[%u]:\n",difficulty);
    DumpHex(merkle_root,crypto_generichash_BYTES);
    unsigned char *out_hash = calloc(crypto_generichash_BYTES, sizeof(unsigned char)); // alocate for hash

    unsigned char ans[5] = {'\0', '\0', '\0', '\0', '\0'}; // 
    unsigned long long ii = 0;
    do
    {
	if (!(ii % 1000000)) printf("->trying %llu'th\n",ii);
	ii++;
        randombytes_buf(CONCAT_BYTES, NONCE_LEN); // take random sample to CONCAT
        crypto_generichash(out_hash, crypto_generichash_BYTES, CONCAT_BYTES, crypto_generichash_BYTES + NONCE_LEN, NULL,
                           0); // calc HASH
    } while ((memcmp(out_hash, ans, difficulty)));
    printf("Calced nonce to match puzzle:\n");
    memcpy(nonce_answ, CONCAT_BYTES, NONCE_LEN);
    DumpHex(nonce_answ, NONCE_LEN);
    free(CONCAT_BYTES);

    test_solution(merkle_root,(char*) nonce_answ, out_hash);
    free(out_hash);
    return nonce_answ;
}
