#include "tests.h"

void test1();

void tests2()
{
    test1();
}

void test1()
{
    printf("testing serialization!\n");
    user_keys uk = create_key_pair();
    signed_message_t msg;
    signed_message_t *msg_p = ls_get_a_signed_msg(uk); // generate random
    msg = *msg_p;
    printf("sended message\n");
    DumpHex(msg.message, msg.length);
    printf("\n");
    char socket_buf_emit[4096] = {0};

    IpcMessage *message = 0;
    message = (IpcMessage *)malloc(sizeof(IpcMessage));
    size_t len = serialize_data_v2(socket_buf_emit, &msg, message);
    DumpHex(socket_buf_emit, len);

    // test deserialization
    print_serialized_data(socket_buf_emit, len);

    free(message);
    free(msg_p);
}
