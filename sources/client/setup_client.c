#include "setup_client.h"
#include "ipc_messages_client.h"

#define BUFSIZE 4096
#define BLOCKSIZE 512
#define BUFSIZEFORMESSAGE 512
#define ring_entries_num 10

IpcMessage *buffer_transactions;
unsigned long long EXPONENT = 9; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

int setup_client_iouring(char* IP_ADDR_TO_CONNECT)
{
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(IP_ADDR_TO_CONNECT, "12345", &hints, &res)))
    {
        printf("getaddrinfo error:%s\n", gai_strerror(status));
        exit(1);
    }
    for (p = res; p != NULL; p = p->ai_next)
    {
        void *addr;

        if (p->ai_family == AF_INET)
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        }
        else
        {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
    }

    int s = socket(res->ai_family, res->ai_socktype,
                   res->ai_protocol); // get a socket
    if (s == -1)
    {
        printf("cannot setup a client socket\n");
        return -1;
    }

    buffer_transactions = calloc(1, sizeof(IpcMessage)); // buffers for storing current buffer size

    struct io_uring ring;
    io_uring_queue_init(ring_entries_num, &ring, 0);

    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);

    io_uring_prep_connect(sqe, s, res->ai_addr, res->ai_addrlen); // prep connect

    if (io_uring_submit(&ring) < 0) // submit now!
        printf("error submitting\n");
    struct io_uring_cqe *cqe_main;
    io_uring_wait_cqe(&ring, &cqe_main);
    int ret = cqe_main->res;
    mtx_lock(&block_created_mtx);
    if (ret==0)
    {
        zlog_info(client_log, "connection to server has been established!");
        printf("connection TO MESSAGE SERVER has been established!\n");
        io_uring_cqe_seen(&ring, cqe_main);
    } else {
        zlog_info(client_log, "error while trying connecting to the server");
	printf("error establishing connection to server, exiting\n");
	close(s);
        free(buffer_transactions);
        freeaddrinfo(res);
	return 0;

    }
    IpcMessage__Status FLAG_FROM_SERVER = IPC_MESSAGE__STATUS__ERROR; // set error default status
    size_t k = 0;
    int flag_block_filled = 0;
    size_t count = 0;
    char *buffer = calloc(BUFSIZE, sizeof(char));
    char *buffer2 = calloc(BUFSIZE, sizeof(char));
    l_msg_container *L_arrays_p_cont  = 0;
    block_t* block_dummy  = 0;
    signed_message_t *msg_arr = calloc(BLOCKSIZE, sizeof(signed_message_t));
    for (size_t i = 0; i < BLOCKSIZE; i++)
    {
        msg_arr[i].message = (unsigned char *)calloc(BUFSIZEFORMESSAGE, sizeof(char)); // asign pointer
    }

    if (client_logging_enabled)
        zlog_info(client_log, "logging client...");
    int ifread = 0;
    for (;;)
    {
// 	printf("cycle!\n");
        struct io_uring_cqe *cqe;
        if (flag_block_filled)
        {
            if (client_logging_enabled)
                zlog_info(client_log, "block (512) messages, has been acquired");
            ifread = 0;
            io_uring_wait_cqe(&ring, &cqe);
	    continue;
            //break;
        }
        if (ifread == 0)
        {
            sqe = io_uring_get_sqe(&ring);                                 // return io entity
            io_uring_prep_recv(sqe, s, buffer, BUFSIZE * sizeof(char), 0); // recv data
            io_uring_submit(&ring);
            io_uring_wait_cqe(&ring, &cqe);
            io_uring_cqe_seen(&ring, cqe);
            ifread = 1;
// 	    continue;
            //break;
        }

        ifread = 0;
        int ret = cqe->res; // N readed bytes
	FLAG_FROM_SERVER = read_response_ONLY_STATUS(buffer, cqe->res);
// 	printf("message flag form server %d\n",FLAG_FROM_SERVER);
        switch (FLAG_FROM_SERVER) 
        {
        case IPC_MESSAGE__STATUS__ASK_NEED_MSG:
            if (client_logging_enabled)
                zlog_info(client_log, "server ask if need msg");
            ret = send_STATUS(&ring, s, buffer2, IPC_MESSAGE__STATUS__NEED_MORE); // BNUFFER2 &&&
            break;
        case IPC_MESSAGE__STATUS__OK:
            if (client_logging_enabled)
                zlog_info(client_log, "accepted a ipc msg from the server %d", cqe->res);
            send_need_more_msg(&ring, cqe->res, buffer);
            break;
        case IPC_MESSAGE__STATUS__MESSAGE_SENDED:
            if (count < BLOCKSIZE - 1)
            {
                if (client_logging_enabled){
                    zlog_info(client_log, "acquired %d,sending ACK...", cqe->res);
                    zlog_info(client_log, "accept msg num: %zu", count);
		}
                deserialize_data_from_server(buffer, cqe->res, msg_arr + count);
                count++;
                ret = send_STATUS(&ring, s, buffer, IPC_MESSAGE__STATUS__ACKN_OK);
            }
            else if (count == BLOCKSIZE - 1)
            {
                if (client_logging_enabled)
                    zlog_info(client_log, "last message has been acquired");
                deserialize_data_from_server(buffer, cqe->res, msg_arr + count);
                if (client_logging_enabled)
                    zlog_info(client_log, "stop, accepting");
                ret = send_STATUS(&ring, s, buffer2, IPC_MESSAGE__STATUS__ENOUGH); // 512 blocks acquired
                 flag_block_filled = 1;
	
    zlog_info(client_log, "calcing merkle tree from received messges!");
    L_arrays_p_cont = calc_merkle_tree(EXPONENT, msg_arr); 
    printf("block of 512 msg has been accepted, calcing merkle root :\n");
    
    DumpHex((*(L_arrays_p_cont->main_layer_pointer[0].main_pointer))->hash, crypto_generichash_BYTES);
    unsigned char* temp_hash = calc_hashof_hash(  (*(L_arrays_p_cont->main_layer_pointer[0].main_pointer))->hash  );// calc hash of hash BIRTHDAY 
    printf("Birthday hash (hash of hash):\n");
    DumpHex(temp_hash,crypto_generichash_BYTES);
    free(temp_hash);
    unsigned char merkle_root_first[crypto_generichash_BYTES];
    memcpy(merkle_root_first,(*(L_arrays_p_cont->main_layer_pointer[0].main_pointer))->hash, crypto_generichash_BYTES);
    //****************************************************/
    printf("-=STARTING PUZZLE SOLVING=-");
    unsigned char* nonce = solve_puzzle(merkle_root_first,3); //calc puzzle
    printf("-=PUZZLE HAS BEEN SOLVED=-");
    //create block
    block_dummy = create_block_dummy(0,merkle_root_first);
    set_nonce_to_block(block_dummy,nonce);
    set_prev_block_hash(block_dummy,NULL);
    // merkle hash
    //     DumpHex( L_arrays_p_cont->main_layer_pointer  (*(L_arrays[0]->main_pointer))->hash  ,
    //     crypto_generichash_BYTES);

    int ver_result = 0;
    zlog_info(client_log, "verifying messages");
    for (size_t i = 0; i < 512; i++)
    { // verify all messages
        // 	  printf("----------------->verify %lu nth\n",i);
        int rez = merkle_verify_message(EXPONENT, i, L_arrays_p_cont->main_layer_pointer);
        if (!rez)
            break;
        ver_result += rez;
    }
	printf("verification result %d\n",ver_result);



	flag_block_created = 1;
    size_t written_block_size = P2P_serialize_block_to_sock(block_dummy,buffer_BLOCK_DATA);
    block_written_size = written_block_size;
    mtx_unlock(&block_created_mtx);
    printf("BLOCK HAS BEEN CREATED!\n");
    printf("--------------------------\n");
//     printf("serialized block is\n");
//     DumpHex(buffer_BLOCK_DATA,written_block_size);
    printf("--------------------------\n");
	flag_block_filled = 1;
            }
            else
            {
                ret = send_STATUS(&ring, s, buffer2, IPC_MESSAGE__STATUS__ERROR); // 512 blocks acquired
            }
            break;
        case IPC_MESSAGE__STATUS__FINISH:
            if (client_logging_enabled)
                zlog_info(client_log, "server finished sending!");
            break;
        case IPC_MESSAGE__STATUS__ALL_BLOCK_MSG_SENDED:
            if (client_logging_enabled)
                zlog_info(client_log, "server finished sending all data messages!");
            ret = send_STATUS(&ring, s, buffer2, IPC_MESSAGE__STATUS__ALL_BLOCK_RECEIVED);
            flag_block_filled = 1;
            break;
        default:
	    printf("default!\n");
            break;
        }

        io_uring_cqe_seen(&ring, cqe);
        k++;
    }
    io_uring_queue_exit(&ring);
    free(buffer);
    free(buffer2);
    teardown_server_sock(s);
    close(s);
    free(buffer_transactions);
    freeaddrinfo(res);

        // free msg for client
    for (size_t i = 0; i < BLOCKSIZE; i++)
    {
        free(msg_arr[i].message);
    }
    // destroy layer
    for (size_t i = 0; i < EXPONENT; i++) // free
    {
        destoroy_a_layer(L_arrays_p_cont->layers_for_destr[i]);
    }
    /*for (size_t i = 0; i < (BLOCKSIZE); i++) // destroy in calling process
    {
        free(msg_arr[i].message);
    }*/
    free(L_arrays_p_cont->layers_for_destr);
    free(L_arrays_p_cont->main_layer_pointer);
    free(L_arrays_p_cont->messages_arr);
    free(msg_arr); // free conrainer for messages
    free(L_arrays_p_cont);
    free(block_dummy);
     // free(msg_arr);
    thrd_exit(1);
}
