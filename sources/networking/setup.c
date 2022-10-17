#include "setup.h"

char *buffers;                   // buffers for incomn connections
size_t *buffer_lengths;          // current buffer length
IpcMessage *buffer_transactions; // buffers for incomn connections
signed_message_t *buffer_signed_message;
signed_message_t** ring_data_buf;
size_t *beffer_sended_N;

void setup_buffers()
{
    buffers = malloc(MAX_CONNECTIONS * BUFFER_SIZE); // connection buffers for messages
    if (!buffers)
        die("malloc");

    buffer_lengths = malloc(MAX_CONNECTIONS * sizeof(size_t)); // buffers for storing current buffer size
    if (!buffer_lengths)
        die("malloc");

    buffer_transactions = calloc(MAX_CONNECTIONS, sizeof(IpcMessage)); // buffers for storing current buffer size
    if (!buffer_transactions)
        die("malloc");

    buffer_signed_message =
        calloc(MAX_CONNECTIONS, sizeof(signed_message_t)); // buffers for storing current buffer size
    if (!buffer_transactions)
        die("malloc");
    beffer_sended_N = calloc(MAX_CONNECTIONS, sizeof(size_t)); // buffers for storing current buffer size
    if (!beffer_sended_N)
        die("malloc");

    ring_data_buf = calloc(512, sizeof(signed_message_t*)); // buffers for storing current buffer size
    if (!ring_data_buf)
        die("malloc");


}

void destroy_buffers()
{
    free(buffers);
    free(buffer_lengths);
}

int setup_serv_sock(uint16_t port)
{
    struct protoent *proto = getprotobyname("tcp");
    int sockfd = socket(AF_INET, SOCK_STREAM, proto->p_proto);
    if (sockfd < 0)
        die("error assigning socket");
    struct sockaddr_in s_addr = {0};
    s_addr.sin_family = AF_INET; // fill sock struct
    s_addr.sin_port = htons(port);
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);                             // ine interfac
                                                                            //
    if (bind(sockfd, (const struct sockaddr *)&s_addr, sizeof(s_addr)) < 0) // relate socket to an adress
        die("error while binding");

    if (listen(sockfd, MAX_CONNECTIONS) < 0)
        die("error while establishing server socket listen");
    return sockfd;
}

// POOLING ONLY WHEN ROOT
void setup_iouring(struct io_uring *ring, int ncon, bool pooling)
{
    int flag = pooling ? IORING_SETUP_SQPOLL | IORING_SETUP_SQ_AFF : 0;
    if ((io_uring_queue_init(ncon, ring, flag)) != 0)
        die("error io uring initialization");
}
