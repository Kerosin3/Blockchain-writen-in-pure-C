#include "tests.h"
#include <assert.h>
#include <sodium/crypto_generichash.h>
#include <sodium/crypto_sign.h>
#include <string.h>
#include <time.h>
#include <tgmath.h>

int test_hpoint_from_2msg_creation();
int test_hashG_node_creation();
int test_process_messages();

void tests(){
	int result = 0;
	/*result+=test_valid_messages();
	result+=test_INvalid_messages();
	result+=test_hash();
	test_hash_merging();*/
	//result+=test_process_messages();
	test_create_level1();
	(!result) ? printf("ALL TESTS PASSED OK\n") : printf("SOME ERRORS WHILE TESTING OCCURRED!\n");
}

int test_create_level1(){
	//create messages
	user_keys uk = create_key_pair();
	unsigned long long n_msg = (1LLU<< 9LLU);
	printf("n msg :%llu\n",n_msg);
	signed_message_t msg_arr[n_msg]; // arr size /2
	for (size_t i = 0; i<n_msg; i++) {
		msg_arr[i] = *ls_get_a_signed_msg(uk); // generate random
		validate_a_message(msg_arr[i],uk.pk);
	//	DumpHex(msg_arr[i].message, msg_arr[i].length);
	}
	layer_hp* L0 =  process_s_messages(n_msg,msg_arr); // messages
	n_msg>>=1LLU;	 // MESSAGES DONE!
	printf("N OF LEVEL 0 HASH NODES %llu\n",n_msg);
	hash_point_p* L0pointer = L0->main_pointer; // hash level 0-1
	// 8 +1
	layer_hp L_arrays[9];
	//L_arrays[8]=*L0; // assign L9
	//for (size_t k=n_msg; k!=0 ; k>>=1LLU ) { 
	for (signed k=8; k>=0 ; --k ) { 
		printf("k is %d \n",k);
		if (k==8) {
		L_arrays[k] =* create_a_h_layer(&n_msg,L0pointer);
		continue;
		}
		L_arrays[k] = *create_a_h_layer(&n_msg,L_arrays[k+1].main_pointer );
	}
//	layer_hp* L1 = create_a_h_layer(&n_msg,L0pointer);

}


int test_process_messages(){

	user_keys uk = create_key_pair();
	unsigned long long n_msg = (1LLU<< 9LLU);
	printf("n msg :%llu\n",n_msg);
	signed_message_t msg_arr[n_msg];
	for (size_t i = 0; i<n_msg; i++) {
		msg_arr[i] = *ls_get_a_signed_msg(uk); // generate random
		validate_a_message(msg_arr[i],uk.pk);
		DumpHex(msg_arr[i].message, msg_arr[i].length);
	}
	//create zero layer
	
	layer_hp* L0 =  process_s_messages(n_msg,msg_arr);

	hash_point_p* L0pointer = L0->main_pointer;

	size_t msg_n_test = 0;
	msg_link a_link;
	get_s_msg_from_L0(L0,msg_n_test);

	size_t ii = 0;
	for (ii = 0; ii<L0->size; ii++) {

		a_link = get_s_msg_from_L0(L0,ii);
		if (!(a_link.msg1 == msg_arr+ii)){
			break;
		} else if (!( a_link.msg2 == msg_arr+(ii + L0->size ) )) {
			break;
		} else {
			continue;
		}
	}
	destoroy_a_layer(L0);
	if (ii == L0->size) return 1;
	return 0;

}


int test_hashG_node_creation(){
	// create 1 ground node with 2 msg
	user_keys uk = create_key_pair();
	signed_message_t *a_msg = get_a_signed_msg(uk);
	printf("message 1 is \n");
	DumpHex(a_msg->message,a_msg->length);
	printf("\n");

	signed_message_t *a_msg2 = get_a_signed_msg(uk);

	if (!validate_a_message(*a_msg,uk.pk))
		printf("validation error!\n");

	if (!validate_a_message(*a_msg2,uk.pk))
		printf("validation error!\n");
	
	hash_point_p aHP1 = create_hpoint_message(a_msg,a_msg2); // created
								 //

	// create 2 ground node with 2 msg
	signed_message_t *a_msg3 = get_a_signed_msg(uk);

	signed_message_t *a_msg4 = get_a_signed_msg(uk);

	if (!validate_a_message(*a_msg3,uk.pk))
		printf("validation error!\n");

	if (!validate_a_message(*a_msg4,uk.pk))
		printf("validation error!\n");
	
	hash_point_p aHP2 = create_hpoint_message(a_msg3,a_msg4); // created
								  //
	// crate G node 
	hash_point_p GNODE = create_hpoint_hashG(aHP1,aHP2);
	printf("test message 1 from GNODE\n");
	if ((  ((hash_point_p) (GNODE->hpoint1))->messages.smsg_p1   ) == a_msg ) {
		printf("validated msg1\n");
	} else {  printf(" msg1 invalid"); }
	if ((  ((hash_point_p) (GNODE->hpoint1))->messages.smsg_p2   ) == a_msg2 ) {
		printf("validated msg2\n");
	} else {  printf(" msg2 invalid"); }
	if ((  ((hash_point_p) (GNODE->hpoint2))->messages.smsg_p1   ) == a_msg3 ) {
		printf("validated msg3\n");
	} else {  printf(" msg3 invalid"); }
	if ((  ((hash_point_p) (GNODE->hpoint2))->messages.smsg_p2   ) == a_msg4 ) {
		printf("validated msg4\n");
	} else {  printf(" msg4 invalid"); }
	return 1;

	//DumpHex( ((hash_point_p) (GNODE->hpoint1))->messages.smsg_p1->message ,  ((hash_point_p) (GNODE->hpoint1))->messages.smsg_p1->length );
	//printf("test message 2 from GNODE\n");
	//DumpHex( ((hash_point_p) (GNODE->hpoint1))->messages.smsg_p1->message ,  ((hash_point_p) (GNODE->hpoint1))->messages.smsg_p1->length );

	return 1;
}
/*
int test_lvl3node_creation(){
	
        user_keys uk = create_key_pair();
	signed_message_t *a_msg = get_a_signed_msg(uk);
	hash_point_p aHP1 = create_hpoint_message(a_msg);	
// 	DumpHex(aHP1->msg_hash,crypto_generichash_BYTES);
	printf("\n");
	if (!validate_a_message(*a_msg,uk.pk))
		printf("validation error!\n");

	user_keys uk2 = create_key_pair();
	signed_message_t *a_msg2 = get_a_signed_msg(uk2);
	hash_point_p aHP2 = create_hpoint_message(a_msg2);	
// 	DumpHex(aHP2->msg_hash,crypto_generichash_BYTES);
	printf("\n");
	if (!validate_a_message(*a_msg2,uk2.pk))
		printf("validation error!\n");
	
	//
	hash_point_p MHP = create_hpoint_hashL1(aHP1, aHP2);

	DumpHex( ((hash_point_p)(MHP->dpointer.hashpointers.hpoint1) )->dpointer.dpointer->message,
	 ((hash_point_p)(MHP->dpointer.hashpointers.hpoint1) )->dpointer.dpointer->length
			);
	
	printf("1, level 2 hash:\n");
	DumpHex(MHP->msg_hash,crypto_generichash_BYTES);

	//-----------------------------------------------//
	user_keys uk3 = create_key_pair();
	signed_message_t *a_msg3 = get_a_signed_msg(uk3);
	hash_point_p aHP3 = create_hpoint_message(a_msg3);	
// 	DumpHex(aHP3->msg_hash,crypto_generichash_BYTES);
	if (!validate_a_message(*a_msg3,uk3.pk))
		printf("validation error!\n");

	user_keys uk4 = create_key_pair();
	signed_message_t *a_msg4 = get_a_signed_msg(uk4);
	hash_point_p aHP4 = create_hpoint_message(a_msg4);	
	if (!validate_a_message(*a_msg4,uk4.pk))
		printf("validation error!\n");
	
	//
	hash_point_p MHP1 = create_hpoint_hashL1(aHP3, aHP4);
	printf("2, level 2 hash:\n");
 	DumpHex(MHP1->msg_hash,crypto_generichash_BYTES);


	// merge to 3 level
		
	hash_point_p MHPX = create_hpoint_hashL1(MHP, MHP1);
	printf("level 3 hash , prev 1:\n");
	DumpHex(MHPX->dpointer.hashpointers.hpoint1,crypto_generichash_BYTES);
	printf("level 3 hash , prev 2:\n");
	DumpHex(MHPX->dpointer.hashpointers.hpoint2,crypto_generichash_BYTES);
	printf("some message %p \n",(MHPX->dpointer.hashpointers.hpoint1));
	printf("length is %llu\n",((hash_point_p)(MHPX->dpointer.hashpointers.hpoint1)) );
	DumpHex( ((hash_point_p)(MHPX->dpointer.hashpointers.hpoint1))->dpointer.dpointer->message, );

}
*/


int test_hpoint_from_2msg_creation(){
	
        user_keys uk = create_key_pair();
	signed_message_t *a_msg = get_a_signed_msg(uk);
	printf("message is\n");
	DumpHex(a_msg->message,a_msg->length);

	user_keys uk2 = create_key_pair();
	signed_message_t *a_msg2 = get_a_signed_msg(uk2);




	if (!validate_a_message(*a_msg,uk.pk))
		printf("validation error!\n");

	if (!validate_a_message(*a_msg2,uk2.pk))
		printf("validation error!\n");
	
	hash_point_p aHP1 = create_hpoint_message(a_msg,a_msg2);	

	printf("check:\n");
	DumpHex(aHP1->messages.smsg_p1->message ,aHP1->messages.smsg_p1->length);
	if  ((a_msg)== aHP1->messages.smsg_p1 )  {
		printf("valid!\n");
		return 1;
	} else {
		return 0 ;
	}


}
/*
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
*/
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
