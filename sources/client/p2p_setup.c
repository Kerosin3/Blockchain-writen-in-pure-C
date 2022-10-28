#include "p2p_setup.h"

char *buffers;                   // buffers for incomn connections
size_t *buffer_lengths;          // current buffer length
size_t *buffer_sended_N;
unsigned char *buffer_BLOCK_DATA;

void setup_buffers()
{
    buffers = malloc(MAX_CONNECTIONS * BUFFER_SIZE); // connection buffers for messages
    if (!buffers)
        die("malloc");

    buffer_lengths = malloc(MAX_CONNECTIONS * sizeof(size_t)); // buffers for storing current buffer size
    if (!buffer_lengths)
        die("malloc");

    buffer_sended_N = calloc(MAX_CONNECTIONS, sizeof(size_t)); // buffers for storing current buffer size
    if (!buffer_sended_N)
        die("malloc");

    buffer_BLOCK_DATA = calloc(BUFFER_SIZE, sizeof(unsigned char)); // buffers for storing current buffer size
    if (!buffer_BLOCK_DATA)
        die("malloc");


    /*buffer_transactions = calloc(MAX_CONNECTIONS, sizeof(IpcMessage)); // buffers for storing current buffer size
    if (!buffer_transactions)
        die("malloc");

    buffer_signed_message =
        calloc(MAX_CONNECTIONS, sizeof(signed_message_t)); // buffers for storing current buffer size
    if (!buffer_transactions)
        die("malloc");
    
    ring_data_buf = calloc(512, sizeof(signed_message_t*)); // buffers for storing current buffer size
    if (!ring_data_buf)
        die("malloc");
*/

}


void destroy_buffers()
{
    free(buffers);
    free(buffer_lengths);
    free(buffer_sended_N);
    free(buffer_BLOCK_DATA);
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
void setup_iouring(struct io_uring *ring, bool pooling)
{
    int flag = pooling ? IORING_SETUP_SQPOLL | IORING_SETUP_SQ_AFF : 0;
    int ret = 0;
    if (  (ret = (io_uring_queue_init(MAX_CONNECTIONS, ring, flag))) != 0){
	 printf("errno:%d\n",errno);
        die("error io uring initialization");
    }
    printf("iouring established!\n");
}

void teardown_server_sock(int servsock)
{
    shutdown(servsock, SHUT_RDWR);
}

