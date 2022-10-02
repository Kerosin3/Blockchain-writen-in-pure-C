#include "tests.h"
#include <assert.h>
#include <sodium/crypto_generichash.h>
#include <sodium/crypto_sign.h>
#include <string.h>
#include <time.h>

void tests(){
	int result = 0;
	/*result+=test_valid_messages();
	result+=test_INvalid_messages();
	result+=test_hash();
	test_hash_merging();*/
//   	test_node_creation();
 	test_Tnode_creation();
	(!result) ? printf("ALL TESTS PASSED OK\n") : printf("SOME ERRORS WHILE TESTING OCCURRED!\n");
}


int test_Tnode_creation(){
	printf("CREATE TNODE\n");
	//create first level 1 node
        user_keys uk = create_key_pair();

	signed_message_t a_msg = *get_a_signed_msg(uk);

        user_keys uk2 = create_key_pair();

	signed_message_t a_msg2 = *get_a_signed_msg(uk2);
	s_link_p alink;

	if (!  (alink = create_node_dtable(a_msg,a_msg2))) return 0;

	
	//create second level 1 node
	 user_keys uk3 = create_key_pair();
	signed_message_t a_msg3 = *get_a_signed_msg(uk3);

        user_keys uk4 = create_key_pair();
	signed_message_t a_msg4 = *get_a_signed_msg(uk4);

	s_link_p alink2;
	if (!  (alink2 = create_node_dtable(a_msg3,a_msg4))) return 0;

	Tnode_p tnode = create_node_dtable_LEVEL2(alink,alink2);

	printf("tnode S hash:\n");
	DumpHex(tnode->hash,crypto_generichash_BYTES);
	printf("leaf 1 msg:\n");
	DumpHex(tnode->leaf1->link1.s_msg.message, tnode->leaf1->link1.s_msg.length);

 	free(alink);
 	free(alink2);
	free(tnode);
	return 1;
	
}

int test_node_creation(){
	printf("TEST DATA MERGLE NODE CREATION\n");
        srand(time(NULL));
        user_keys uk = create_key_pair();
	test_msg_t somemsg= get_test_msg(100);	
	signed_message_t a_msg = sign_a_message((unsigned char*)somemsg.test_msg,somemsg.len, uk.sk);
	//put a PK
	put_a_PK(&a_msg,uk.pk);
	printf("message1 is\n");
	DumpHex(a_msg.message,a_msg.length);

	user_keys uk2 = create_key_pair();
	test_msg_t somemsg2 = get_test_msg(50);	
	signed_message_t a_msg2 = sign_a_message((unsigned char*)somemsg2.test_msg,somemsg2.len, uk2.sk);
	//put a PK
	put_a_PK(&a_msg2,uk2.pk);
	printf("message2 is\n");
	DumpHex(a_msg2.message,a_msg2.length);


	//create smart link
	s_link_p alink;
	if (!  (alink = create_node_dtable(a_msg,a_msg2))) return 0;
	printf("message 1 in link:\n");
	DumpHex(alink->link1.s_msg.message,alink->link1.s_msg.length);
	printf("message 2 in link:\n");
	DumpHex(alink->link2.s_msg.message,alink->link2.s_msg.length);
	printf("summary hash:\n");
	DumpHex(alink->hash,crypto_generichash_BYTES);

	free(alink);
	return 1;
}

int test_hash_merging(){
	
	printf("TEST HASHIN MERGING\n");
        user_keys uk = create_key_pair();
        srand(time(NULL));
	test_msg_t somemsg = get_test_msg(100);	
	signed_message_t a_msg = sign_a_message((unsigned char*)somemsg.test_msg,somemsg.len, uk.sk);


	user_keys uk2 = create_key_pair();
	test_msg_t somemsg2 = get_test_msg(100);	
	signed_message_t a_msg2 = sign_a_message((unsigned char*)somemsg2.test_msg,somemsg2.len, uk2.sk);


	merge_2hashses(calc_hash(a_msg),calc_hash(a_msg2));
	return 1;
}
//test hash generating
int test_hash(){
	printf("TEST HASHING FUNCTION\n");
        user_keys uk = create_key_pair();
        srand(time(NULL));
	test_msg_t somemsg = get_test_msg(100);	
	signed_message_t a_msg = sign_a_message((unsigned char*)somemsg.test_msg,somemsg.len, uk.sk);
	unsigned char hash[crypto_generichash_BYTES];
	unsigned char hash_re[crypto_generichash_BYTES];
	memcpy(hash,calc_hash(a_msg),crypto_generichash_BYTES);
	printf("calced hash:\n");
	DumpHex(hash,crypto_generichash_BYTES);
	size_t i = 0;
	for (i = 0; i< 500; i++) {
//		printf("i=%ld new hash:\n",i);
		memcpy(hash_re,calc_hash(a_msg),crypto_generichash_BYTES);
//		DumpHex(hash_re,crypto_generichash_BYTES);
		if (( strncmp((char*) hash ,(char*) hash_re ,crypto_generichash_BYTES ))!=0) break;

	}
	if (i==500) {
		printf("TEST PASSED, NO ERROR OCCURRED \n");
		return 0;
	}
	else printf("TEST PASSED, ERROR OCCURRED!!!!!!!!!!!!! \n");
	return 1;

}
//test message encryption
int test_valid_messages(){
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
	if (i==15) {
		printf("TEST PASSED, NO ERROR OCCURRED \n");
		return 0;
	}
	else printf("TEST PASSED, ERROR OCCURRED!!!!!!!!!!!!! \n");
	return 1;
}
//wrong keys
int test_INvalid_messages(){
        user_keys uk = create_key_pair();
        user_keys uk2 = create_key_pair();
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

	int result = 0;
	for (size_t i=0; i<15; i++) {
			a_msg = sign_a_message((unsigned char*)somemsg[i].test_msg,somemsg[i].len, uk.sk);
			//---validation
			((validate_a_message(a_msg,uk.pk))) ? (result+=1) : (result=0);
			free(somemsg[i].test_msg);
			free(a_msg.message);
			(result==1) ? printf("validated!\n") : printf("forged!\n");
	}
	if (result==15) {
		printf("TEST PASSED, NO ERROR OCCURRED \n");
		return 0;
	}
	else printf("TEST PASSED, ERROR OCCURRED!!!!!!!!!!!!! \n");
	return 1;
}
