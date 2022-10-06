#include "puzzle.h"
#include <sodium/randombytes.h>
#include <string.h>


unsigned char ans[5] = {'\0','\0','\0','\0','\0'};

const unsigned char puzzle_msg[10] = { 't','e','s','t','p','u','z','z','l','e' };

void solve_puzle(size_t diff_level){
	unsigned char out_hash[crypto_generichash_BYTES];
	crypto_generichash(out_hash,crypto_generichash_BYTES,
			puzzle_msg, 10,
			NULL,0);

 	unsigned char solution[crypto_generichash_BYTES] = {0};

	if (sodium_init() < 0 ) die("libsodium!"); 
	    randombytes_buf(random, 10);
 	memcpy(solution,out_hash, crypto_generichash_BYTES); // concat
	solution[0]=0;
	DumpHex(out_hash, crypto_generichash_BYTES);	
// 	DumpHex(solution, crypto_generichash_BYTES+1);	

	unsigned char nonce[5] = {0};
	unsigned char try_data[crypto_generichash_BYTES] = {0};
	do {
			
	}while (!(strcmp((char*)out_hash,(char*) solution )));
}

void solve_puzlev2(size_t diff_level){

	printf("try solve\n");
	unsigned char random[10] = {0};
	unsigned char out_hash[crypto_generichash_BYTES] = {0};
	if (sodium_init() < 0 ) die("libsodium!"); 
	do {
		randombytes_buf(random, 10); // generate radom
		crypto_generichash(out_hash,crypto_generichash_BYTES,
			random, 10,
			NULL,0);

	}while( memcmp(out_hash,ans,diff_level)); 
	printf("answer:\n");
	DumpHex(out_hash,10);

}
