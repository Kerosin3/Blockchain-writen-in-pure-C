#include "tests.h"
#include <assert.h>
#include <sodium/crypto_sign.h>
#include <time.h>

void test_valid_messages(){
        user_keys uk = create_key_pair();
	DumpHex(uk.sk,crypto_sign_SECRETKEYBYTES);
	DumpHex(uk.pk, crypto_sign_PUBLICKEYBYTES) ;
	printf("keys have been created\n");
	signed_message_t a_msg;
	//char* somemsg = "asdfghj";
        srand(time(NULL));
	size_t i = 0;
	for (i=0; i<15; i++) {
		int result = 0;
		test_msg_t somemsg = get_test_msg(15+i);	
		//DumpHex(somemsg.test_msg, somemsg.len);
		for (size_t ii=0; ii<100; ii++) {
			a_msg = sign_a_message((unsigned char*)somemsg.test_msg,somemsg.len, uk.sk);
			//---validation
			((validate_a_message(a_msg,uk.pk))) ? (result=1) : (result=0);
			if (!result) break;
			
		}
			free(somemsg.test_msg);
			free(a_msg.message);
			(result==1) ? printf("validated!\n") : printf("forged!\n");
			if (!result) break;
	}
	if (i==15) printf("TEST PASSED, NO ERROR ORCCURED \n");
	else printf("TEST PASSED, ERROR ORCCURED!!!!!!!!!!!!! \n");

}

void test_INvalid_messages(){
        user_keys uk = create_key_pair();
	DumpHex(uk.sk,crypto_sign_SECRETKEYBYTES);
	DumpHex(uk.pk, crypto_sign_PUBLICKEYBYTES) ;
	printf("keys have been created\n");
	signed_message_t a_msg;
	//char* somemsg = "asdfghj";
	
        srand(time(NULL));
	test_msg_t somemsg[15];
	for (size_t k=0; k<15; k++) {
		somemsg[k] = get_test_msg(15);	
	}

	for (size_t i=0; i<15; i++) {
		int result = 0;
		for (size_t ii=0; ii<100; ii++) {
			a_msg = sign_a_message((unsigned char*)somemsg[i].test_msg,somemsg[i].len, uk.sk);
			//---validation
			((validate_a_message(a_msg,uk.pk))) ? (result=1) : (result=0);
			if (!result) break;
			
		}
			free(somemsg[i].test_msg);
			free(a_msg.message);
			(result==1) ? printf("validated!\n") : printf("forged!\n");
	}

}
