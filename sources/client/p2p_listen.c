#include <p2p_listen.h>

#define ring_entries_num 10

typedef enum {
	TEST = 0,
	ACCEPTED_PING,
	SEND_PONG,
	PROCESS_RESPONSE,
	ASK_FOR_A_BLOCK,
	PROCESS_BLOCK_REQUEST,
	READ_RESPONSE,
} cl_p2p_state;

void cl_p2p_send_STATUS(struct io_uring *ring, int client_fd, P2pIpcMessage__Status STATUS,char* buffer_write,cl_p2p_state STATE_MACHINE   );
static cl_p2p_state request_data_event_type(uint64_t request_data);
static u_int64_t make_request_data(int client_fd, cl_p2p_state flag);
void cl_P2P_read_status_response(struct io_uring *ring, int client_fd, char* buffer , cl_p2p_state STATE_MACHINE);



void setup_p2p_listening(char* IP_ADD_LISTEN)
{

    mtx_lock(&peer_connection_accepted);
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo( IP_ADD_LISTEN,"10001", &hints, &res)))
    {
    	if (p2p_logging_enabled) zlog_info(p2p_log, "getaddrinfo error:%s\n", gai_strerror(status));
    	thrd_exit(1);
        //exit(1);
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
    }else {
	    printf("established socket for incoming p2p connections!\n");
    }

    if (p2p_logging_enabled) zlog_info(p2p_log, "p2p client tryint to connect...");

    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    	if (p2p_logging_enabled) zlog_info(p2p_log,"error setting socket reuse"); 
    int rezopt = fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK);
    if (rezopt == -1){
    	if (p2p_logging_enabled) zlog_info(p2p_log,"error error calling fcntl"); 
    }
    if (p2p_logging_enabled) zlog_info(p2p_log, "have setted socket nonblock...");
    if (p2p_logging_enabled) zlog_info(p2p_log, "locking mutex connection accepted...");
    int done=1;
    while(1){
	     done = connect(s,res->ai_addr,res->ai_addrlen);
	     if (!done) break ;
	     printf("trying to connect to neighboor node\n");
	     sleep(2);
    }
    mtx_unlock(&peer_connection_accepted);
    if (p2p_logging_enabled) zlog_info(p2p_log, "setting socket nonblock...");
    printf("GOT CONNECTION!\n");
    if (p2p_logging_enabled) zlog_info(p2p_log, "got connection to p2p server");
    struct io_uring ring;
    io_uring_queue_init(ring_entries_num, &ring, 0);

    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);

    char* buffer = calloc(DEFAULT_BUF_SIZE, sizeof(char));
    char* buffer_send = calloc(DEFAULT_BUF_SIZE, sizeof(char));

    struct io_uring_cqe *cqe;
    sqe = io_uring_get_sqe(&ring);                                 // return io entity
    io_uring_prep_recv(sqe, s, buffer, DEFAULT_BUF_SIZE * sizeof(char), 0); // WAITING PING
    size_t cycle = 0;
    int flag_ready_count = 0;
    io_uring_sqe_set_data64(sqe, make_request_data(s, PROCESS_RESPONSE)); // set first wait
    if (io_uring_submit(&ring) < 0) {// submit 
        if (p2p_logging_enabled) zlog_info(p2p_log, "error submitting");
    }
    for (;;){
        if (kill_thread_p2p) break;// kill thread
    	io_uring_wait_cqe(&ring, &cqe);
	cl_p2p_state STATE = request_data_event_type(cqe->user_data);
 	P2pIpcMessage__Status msg_status;
// 	printf("message status %d\n",STATE);
	switch (STATE) { //INTERNAL STATE
		case PROCESS_RESPONSE:  // RECEIVING A MESSAGE
	                msg_status = P2P_deserialize_STATUS( buffer, cqe->res);
// 			DumpHex(buffer,cqe->res);
// 			printf("testing message with P2P status %d\n",msg_status);
			switch (msg_status) {
				case P2P__IPC_MESSAGE__STATUS__PING: // server sent PING -> answer pong
					cl_p2p_send_STATUS(&ring,s,P2P__IPC_MESSAGE__STATUS__PONG, buffer_send, READ_RESPONSE);
					printf("PONG SENDED, WAITING OK!\n");
        				if (p2p_logging_enabled) zlog_info(p2p_log, "p2p client pong sended");
					break;
				case P2P__IPC_MESSAGE__STATUS__OK:
        				if (p2p_logging_enabled) zlog_info(p2p_log, "p2p server answered OK");
					printf("SERVER ANSWERED OK\n");
					cl_p2p_send_STATUS(&ring,s,P2P__IPC_MESSAGE__STATUS__ASK_IF_BLOCK_READY, buffer_send,READ_RESPONSE);
					printf("SENDED BLOCK REQUEST\n");
        				if (p2p_logging_enabled) zlog_info(p2p_log, "p2p client asks for a block");
					break;
				case P2P__IPC_MESSAGE__STATUS__BLOCK:
					printf("BLOCK ACCEPTED\n");
					// deresialize here
					block_t* recv_block = deserialize_block(buffer,cqe->res);
					print_block_data(recv_block);
        				if (p2p_logging_enabled) zlog_info(p2p_log, "block hash been accepted");
					break;
				case P2P__IPC_MESSAGE__STATUS__BLOCK_NOT_READY:
					//wait till block is ready!
					printf("getting answer: BLOCK IS NOT READY!\n");
        				if (p2p_logging_enabled) zlog_info(p2p_log, "block is not ready yet");
					printf("asking again if block is ready\n");
        				if (p2p_logging_enabled) zlog_info(p2p_log, "asking again if block is ready");
					if (flag_ready_count){
						printf("sleeping for new ask for ready block\n");
						sleep(2);
						cl_p2p_send_STATUS(&ring,s,P2P__IPC_MESSAGE__STATUS__ASK_IF_BLOCK_READY, buffer_send,READ_RESPONSE);
					} else {
						cl_p2p_send_STATUS(&ring,s,P2P__IPC_MESSAGE__STATUS__ASK_IF_BLOCK_READY, buffer_send,READ_RESPONSE);
						flag_ready_count=1;
					}
					break;
				default:
					printf("waiting default!\n");
					break;
					
			}
			break;
		case READ_RESPONSE:
// 			printf("reading response\n");
			cl_P2P_read_status_response(&ring,s,buffer,PROCESS_RESPONSE);
        		if (p2p_logging_enabled) zlog_info(p2p_log, "reading p2p server response");
			break;

		/*case ASK_FOR_A_BLOCK:
		break;
		case PROCESS_BLOCK_REQUEST:
			printf("make recv request\n");
			break;*/
		default: 
			printf("default!\n");
			break;
	}
    	io_uring_cqe_seen(&ring, cqe);
 	cycle++;
    }
    io_uring_queue_exit(&ring);
    close(s);
    free(buffer);
    free(buffer_send);
    freeaddrinfo(res);
    thrd_exit(1);
}

void cl_P2P_read_status_response(struct io_uring *ring, int client_fd, char* buffer , cl_p2p_state STATE_MACHINE)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    io_uring_prep_recv(sqe, client_fd, buffer, BUFFER_SIZE, 0);
//     DumpHex(get_client_buffer(client_fd),wr_len);
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, STATE_MACHINE));
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
}


void cl_p2p_send_STATUS(struct io_uring *ring, int client_fd, P2pIpcMessage__Status STATUS,char* buffer_write,cl_p2p_state STATE_MACHINE   )
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
   //                                                   IPC_MESSAGE__STATUS__ASK_NEED_MSG); // write to client buffer
    size_t wr_len =  P2P_send_status(STATUS,buffer_write);
    io_uring_prep_send(sqe, client_fd, buffer_write, wr_len,
                       MSG_DONTWAIT);                                          // send a message

    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, STATE_MACHINE));
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    if (p2p_logging_enabled) zlog_info(p2p_log, "sent PONG msg");
}


u_int64_t make_request_data(int client_fd, cl_p2p_state flag)
{
    return (uint64_t)(flag) << 32 | client_fd;
}

cl_p2p_state request_data_event_type(uint64_t request_data)
{
    uint64_t mask = ULLONG_MAX - UINT_MAX;
    // ULLONG_MAX - UINT_MAX = 0xFFFFFFFF00000000
    uint64_t ret = (request_data & (mask)) >> 32;
    return (cl_p2p_state)ret;
}
