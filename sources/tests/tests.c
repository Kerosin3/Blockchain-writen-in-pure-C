#include "tests.h"
#include <assert.h>
#include <sodium/crypto_generichash.h>
#include <sodium/crypto_sign.h>
#include <string.h>
#include <tgmath.h>
#include <time.h>

int test_hpoint_from_2msg_creation();
int test_hashG_node_creation();
int test_process_messages();

int test_create_tree();
void tests()
{
    int result = 0;
    /*result+=test_valid_messages();
    result+=test_INvalid_messages();
    result+=test_hash();
    test_hash_merging();*/
    // result+=test_process_messages();
    //	test_create_tree();
    // solve_puzlev2(2);
    //	setup_client();

     start_server(12345);
    //
    //
    // test_cleanup_message();
    // test_message_creation_and_validation();
    //	test_create_and_destroy_hpoint();
    // test_process_messages_L1_v2();
//     create_test_messages(10);
    (!result) ? printf("ALL TESTS PASSED OK\n") : printf("SOME ERRORS WHILE TESTING OCCURRED!\n");
}
// ok!
int test_message_creation_and_validation()
{

    user_keys uk = create_key_pair();
    signed_message_t **msg_arr = calloc(512, sizeof(signed_message_t *));
    size_t i = 0;
    int rez = 0;
    for (i = 0; i < 512; i++)
    {
        msg_arr[i] = ls_get_a_signed_msg(uk);
        rez += validate_a_message(*msg_arr[i], uk.pk);
    }
    printf("rez is %d\n", rez);

    for (i = 0; i < 512; i++)
    {
        destroy_signed_message(msg_arr[i]);
    }
    free(msg_arr);
    return (rez == 512) ? 0 : 1;
}

// ok!
int test_cleanup_message()
{

    user_keys uk = create_key_pair();
    signed_message_t *msg;
    msg = ls_get_a_signed_msg(uk); // generate random
    DumpHex(msg->message, msg->length);
    validate_a_message(*msg, uk.pk);
    destroy_signed_message(msg);
}

// ok!
int test_create_and_destroy_hpoint()
{

    user_keys uk = create_key_pair();
    signed_message_t *msg1;
    msg1 = ls_get_a_signed_msg(uk); // generate random
    printf("message 1\n");
    DumpHex(msg1->message, msg1->length);
    validate_a_message(*msg1, uk.pk);

    user_keys uk2 = create_key_pair();
    signed_message_t *msg2;
    msg2 = ls_get_a_signed_msg(uk2); // generate random
    printf("message 2\n");
    DumpHex(msg2->message, msg2->length);
    validate_a_message(*msg2, uk2.pk);

    hash_point_p HASH_POINT = 0;
    HASH_POINT = create_hpoint_message(msg1, msg2);
    int rez = 0;
    if (HASH_POINT->hpoint1 == msg1)
        rez++;
    if (HASH_POINT->hpoint2 == msg2)
        rez++;
    destroy_signed_message(msg1);
    destroy_signed_message(msg2);
    destroy_hpoint_message(HASH_POINT);
    return (rez == 2) ? 0 : 1;
}
// OK
int test_process_messages_L1_v2()
{
    unsigned long long n_msg = (1LLU << 5LLU); //  create 2^9 messages
    user_keys uk = create_key_pair();
    signed_message_t **msg_arr = calloc(n_msg, sizeof(signed_message_t *));
    size_t i = 0;
    int rez = 0;
    for (i = 0; i < n_msg; i++)
    {
        msg_arr[i] = ls_get_a_signed_msg(uk); // pointer to message
        rez += validate_a_message(*msg_arr[i], uk.pk);
    }
    layer_hp *root_layer = process_s_messagesV2(n_msg, msg_arr);

    destoroy_a_layer(root_layer); // destroy level
    printf("rez is %d\n", rez);   // destoy created messages
    for (i = 0; i < n_msg; i++)
    {
        destroy_signed_message(msg_arr[i]);
    }
    free(msg_arr); // free conrainer for messages

    return (rez == n_msg) ? 0 : 1;
}

int test_create_tree()
{
    // create messages
    user_keys uk = create_key_pair();
    unsigned long long MSG_EXPONENT = 6LLU;
    layer_hp *L_arrays[MSG_EXPONENT];
    layer_hp L_arrays_p[MSG_EXPONENT]; // for free

    unsigned long long n_msg = (1LLU << MSG_EXPONENT); // level 0
    printf("N msg %llu\n", n_msg);

    L_arrays[MSG_EXPONENT - 1] = create_LEVEL0(&n_msg, uk); // need to free messages!
    L_arrays_p[MSG_EXPONENT - 1] = *L_arrays[MSG_EXPONENT - 1];

    fill_intermediate_levels(MSG_EXPONENT, &n_msg, L_arrays, L_arrays_p); // done
    /*
    size_t ii = 0;
    msg_link a_link;
    for (ii = 0; ii< L_arrays[MSG_EXPONENT-1]->size; ii++) {
        a_link = get_s_msg_from_L0( L_arrays[MSG_EXPONENT-1],ii );
        if (!(a_link.msg1 == (msg_arr_p)+ii)){
            break;
        } else if (!( a_link.msg2 == msg_arr_p+(ii + L_arrays[(MSG_EXPONENT-1)]->size ) )) {
            break;
        } else {
            continue;
        }
    }
    if (ii != 256) return 0;
    printf("ii is %lu\n",ii);
    */
    //---------------------------------------------
    // check others layer
    printf("getting msg from root node\n");
    DumpHex(get_a_hashes_Hnode(L_arrays, 0).Shash, crypto_generichash_BYTES);
    printf("\n");
    DumpHex(get_a_hashes_Hnode(L_arrays, 0).hash1, crypto_generichash_BYTES);
    printf("\n");
    DumpHex(get_a_hashes_Hnode(L_arrays, 0).hash2, crypto_generichash_BYTES);
    // check others layer
    printf("getting msg from root node\n");
    DumpHex(get_a_hashes_Hnode(L_arrays + 1, 0).Shash, crypto_generichash_BYTES);
    printf("\n");
    DumpHex(get_a_hashes_Hnode(L_arrays + 1, 0).hash1, crypto_generichash_BYTES);
    printf("\n");
    DumpHex(get_a_hashes_Hnode(L_arrays + 1, 0).hash2, crypto_generichash_BYTES);

    /*
    printf("root pointer\n");
    DumpHex( (*(L_arrays[0]->main_pointer))->hash, crypto_generichash_BYTES);
    printf("check hash from ouside side 1\n");
    DumpHex(  (((hash_point_p) ((*(L_arrays[0]->main_pointer))->hpoint1)))  , crypto_generichash_BYTES);
    printf("check hash from ouside side 2\n");
    DumpHex(  (((hash_point_p) ((*(L_arrays[0]->main_pointer))->hpoint2)))  , crypto_generichash_BYTES);
    */
    for (size_t i = 0; i < MSG_EXPONENT; i++)
    {
        destoroy_a_layer(L_arrays[i]);
    }
    /*
        for (size_t i =0; i< n_msg; i++) {
            free( msg_arr_p[i].message );
        }
    */
    //	layer_hp* L1 = create_a_h_layer(&n_msg,L0pointer);
}

int test_process_messages()
{

    user_keys uk = create_key_pair();
    unsigned long long n_msg = (1LLU << 9LLU);
    printf("n msg :%llu\n", n_msg);
    signed_message_t msg_arr[n_msg];
    for (size_t i = 0; i < n_msg; i++)
    {
        msg_arr[i] = *ls_get_a_signed_msg(uk); // generate random
        validate_a_message(msg_arr[i], uk.pk);
        DumpHex(msg_arr[i].message, msg_arr[i].length);
    }
    // create zero layer

    layer_hp *L0 = process_s_messages(n_msg, msg_arr);

    hash_point_p *L0pointer = L0->main_pointer;

    size_t msg_n_test = 0;
    msg_link a_link;
    get_s_msg_from_L0(L0, msg_n_test);

    size_t ii = 0;
    for (ii = 0; ii < L0->size; ii++)
    {

        a_link = get_s_msg_from_L0(L0, ii);
        if (!(a_link.msg1 == msg_arr + ii))
        {
            break;
        }
        else if (!(a_link.msg2 == msg_arr + (ii + L0->size)))
        {
            break;
        }
        else
        {
            continue;
        }
    }
    printf("ii is %lu\n", ii);
    destoroy_a_layer(L0);
    if (ii == L0->size)
        return 1;
    return 0;
}

int test_hashG_node_creation()
{
    // create 1 ground node with 2 msg
    user_keys uk = create_key_pair();
    signed_message_t *a_msg = get_a_signed_msg(uk);
    printf("message 1 is \n");
    DumpHex(a_msg->message, a_msg->length);
    printf("\n");

    signed_message_t *a_msg2 = get_a_signed_msg(uk);

    if (!validate_a_message(*a_msg, uk.pk))
        printf("validation error!\n");

    if (!validate_a_message(*a_msg2, uk.pk))
        printf("validation error!\n");

    hash_point_p aHP1 = create_hpoint_message(a_msg, a_msg2); // created
                                                              //

    // create 2 ground node with 2 msg
    signed_message_t *a_msg3 = get_a_signed_msg(uk);

    signed_message_t *a_msg4 = get_a_signed_msg(uk);

    if (!validate_a_message(*a_msg3, uk.pk))
        printf("validation error!\n");

    if (!validate_a_message(*a_msg4, uk.pk))
        printf("validation error!\n");

    hash_point_p aHP2 = create_hpoint_message(a_msg3, a_msg4); // created
                                                               //
    // crate G node
    hash_point_p GNODE = create_hpoint_hashG(aHP1, aHP2);
    printf("test message 1 from GNODE\n");
    if ((((hash_point_p)(GNODE->hpoint1))->messages.smsg_p1) == a_msg)
    {
        printf("validated msg1\n");
    }
    else
    {
        printf(" msg1 invalid");
    }
    if ((((hash_point_p)(GNODE->hpoint1))->messages.smsg_p2) == a_msg2)
    {
        printf("validated msg2\n");
    }
    else
    {
        printf(" msg2 invalid");
    }
    if ((((hash_point_p)(GNODE->hpoint2))->messages.smsg_p1) == a_msg3)
    {
        printf("validated msg3\n");
    }
    else
    {
        printf(" msg3 invalid");
    }
    if ((((hash_point_p)(GNODE->hpoint2))->messages.smsg_p2) == a_msg4)
    {
        printf("validated msg4\n");
    }
    else
    {
        printf(" msg4 invalid");
    }
    return 1;

    // DumpHex( ((hash_point_p) (GNODE->hpoint1))->messages.smsg_p1->message ,  ((hash_point_p)
    // (GNODE->hpoint1))->messages.smsg_p1->length ); printf("test message 2 from GNODE\n"); DumpHex( ((hash_point_p)
    // (GNODE->hpoint1))->messages.smsg_p1->message ,  ((hash_point_p) (GNODE->hpoint1))->messages.smsg_p1->length );

    return 1;
}

int test_hpoint_from_2msg_creation()
{

    user_keys uk = create_key_pair();
    signed_message_t *a_msg = get_a_signed_msg(uk);
    printf("message is\n");
    DumpHex(a_msg->message, a_msg->length);

    user_keys uk2 = create_key_pair();
    signed_message_t *a_msg2 = get_a_signed_msg(uk2);

    if (!validate_a_message(*a_msg, uk.pk))
        printf("validation error!\n");

    if (!validate_a_message(*a_msg2, uk2.pk))
        printf("validation error!\n");

    hash_point_p aHP1 = create_hpoint_message(a_msg, a_msg2);

    printf("check:\n");
    DumpHex(aHP1->messages.smsg_p1->message, aHP1->messages.smsg_p1->length);
    if ((a_msg) == aHP1->messages.smsg_p1)
    {
        printf("valid!\n");
        return 1;
    }
    else
    {
        return 0;
    }
}
/*
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
*/
