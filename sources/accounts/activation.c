#include "activation.h"
#include <string.h>

#define MESSAGE (const unsigned char *) "testasdf"
#define MESSAGE_LEN 8



void DumpHex(const void *data, size_t size);

unsigned char* sign_a_message(unsigned char* msg,size_t len,unsigned char* secret_key){
	unsigned char *signed_message = calloc(crypto_sign_BYTES+len, sizeof(char));
	unsigned long long signed_message_len;
	crypto_sign(signed_message, &signed_message_len,
            msg, len, secret_key);
	printf("message signed!\n");
	return signed_message;
}

void test(unsigned char* some_msg){
	user_keys some_key = create_key_pair();
	unsigned char* sgn_msg = sign_a_message(some_msg,sizeof(some_key),some_key.sk);
	printf("%s\n",sgn_msg);
}

void some(){
unsigned char pk[crypto_sign_PUBLICKEYBYTES];
unsigned char sk[crypto_sign_SECRETKEYBYTES];
crypto_sign_keypair(pk, sk);

unsigned char signed_message[crypto_sign_BYTES + MESSAGE_LEN];
unsigned long long signed_message_len;

crypto_sign(signed_message, &signed_message_len,
            MESSAGE, MESSAGE_LEN, sk);
DumpHex(signed_message, signed_message_len);
unsigned char unsigned_message[MESSAGE_LEN];
unsigned long long unsigned_message_len;
if (crypto_sign_open(unsigned_message, &unsigned_message_len,
                     signed_message, signed_message_len,(unsigned char*) "dasdasda") != 0) {
    /* incorrect signature! */
    printf("incorrect signature!\n");
    return;
}
DumpHex(unsigned_message, unsigned_message_len);
printf("\n--->%s||||\n",unsigned_message);
}

/*
 * for debugging
 * */
void DumpHex(const void *data, size_t size) {
  char ascii[17];
  size_t i, j;
  ascii[16] = '\0';
  for (i = 0; i < size; ++i) {
    printf("%02X ", ((unsigned char *)data)[i]);
    if (((unsigned char *)data)[i] >= ' ' &&
        ((unsigned char *)data)[i] <= '~') {
      ascii[i % 16] = ((unsigned char *)data)[i];
    } else {
      ascii[i % 16] = '.';
    }
    if ((i + 1) % 8 == 0 || i + 1 == size) {
      printf(" ");
      if ((i + 1) % 16 == 0) {
        printf("|  %s \n", ascii);
      } else if (i + 1 == size) {
        ascii[(i + 1) % 16] = '\0';
        if ((i + 1) % 16 <= 8) {
          printf(" ");
        }
        for (j = (i + 1) % 16; j < 16; ++j) {
          printf("   ");
        }
        printf("|  %s \n", ascii);
      }
    }
  }
}
/*
void some(){
unsigned char alice_publickey[crypto_box_PUBLICKEYBYTES];
unsigned char alice_secretkey[crypto_box_SECRETKEYBYTES];
crypto_box_keypair(alice_publickey, alice_secretkey);

unsigned char bob_publickey[crypto_box_PUBLICKEYBYTES];
unsigned char bob_secretkey[crypto_box_SECRETKEYBYTES];
crypto_box_keypair(bob_publickey, bob_secretkey);

unsigned char nonce[crypto_box_NONCEBYTES];
unsigned char ciphertext[CIPHERTEXT_LEN];
randombytes_buf(nonce, sizeof nonce);
if (crypto_box_easy(ciphertext, MESSAGE, MESSAGE_LEN, nonce,
                    bob_publickey, alice_secretkey) != 0) {
//    error 
}
printf("cyphered text :%s\n",ciphertext);

unsigned char decrypted[MESSAGE_LEN];
if (crypto_box_open_easy(decrypted, ciphertext, CIPHERTEXT_LEN, nonce,
                         alice_publickey, bob_secretkey) != 0) {
    // message for Bob pretending to be from Alice has been forged! 
}
	printf("decrypted text is %s\n",decrypted);
}
*/

