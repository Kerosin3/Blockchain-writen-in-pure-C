#include <p2p_listen.h>

#define ring_entries_num 10

typedef enum {
	TEST = 0,
	ACCEPTED_PING,
	SEND_PONG,
	WAITING,
	ASK_FOR_A_BLOCK,
} cl_p2p_state;

static cl_p2p_state request_data_event_type(uint64_t request_data);
static u_int64_t make_request_data(int client_fd, cl_p2p_state flag);
void cl_p2p_send_STATUS(struct io_uring *ring, int client_fd, P2pIpcMessage__Status STATUS,char* buffer_write  );

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
	    printf("one\n");
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
	     printf("trying to connect againg\n");
	     sleep(2);
    }
    mtx_unlock(&peer_connection_accepted);
    if (p2p_logging_enabled) zlog_info(p2p_log, "setting socket nonblock...");
    printf("GOT CONNECTION!\n");
    if (p2p_logging_enabled) zlog_info(p2p_log, "got connection to p2p server");
    struct io_uring ring;
    io_uring_queue_init(ring_entries_num, &ring, 0);

    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);

    P2pIpcMessage__Status STATUS_OF_MESSAGE;
    char* buffer = calloc(4096, sizeof(char));
    char* buffer_send = calloc(4096, sizeof(char));

    struct io_uring_cqe *cqe;
    sqe = io_uring_get_sqe(&ring);                                 // return io entity
    io_uring_prep_recv(sqe, s, buffer, 4096 * sizeof(char), 0); // WAITING PING
    size_t cycle = 0;
    io_uring_sqe_set_data64(sqe, make_request_data(s, WAITING)); // set first wait
    if (io_uring_submit(&ring) < 0) {// submit 
        if (p2p_logging_enabled) zlog_info(p2p_log, "error submitting");
    }
    for (;;){
        if (kill_thread_p2p) break;
    	io_uring_wait_cqe(&ring, &cqe);
	cl_p2p_state STATE = request_data_event_type(cqe->user_data);
 	P2pIpcMessage__Status msg_status;
	switch (STATE) { //INTERNAL STATE
		case WAITING:  // RECEIVING A MESSAGE
	                msg_status = P2P_deserialize_STATUS( buffer, cqe->res);
			DumpHex(buffer,cqe->res);
			printf("testing message with status %d\n",msg_status);
			switch (msg_status) {
				case P2P__IPC_MESSAGE__STATUS__PING: // server sent PING -> answer pong
					cl_p2p_send_STATUS(&ring,s,P2P__IPC_MESSAGE__STATUS__PONG, buffer_send);
    					io_uring_sqe_set_data64(sqe, make_request_data(s, ASK_FOR_A_BLOCK));
					printf("PONG SENDED, asking for a block!\n");
        				if (p2p_logging_enabled) zlog_info(p2p_log, "pong sended");
					break;
//				case P2P__IPC_MESSAGE__STATUS__ASK_IF_BLOCK_READY:
//					printf(" I am asked if block is ready\n");
//    					io_uring_sqe_set_data64(sqe, make_request_data(s, WAITING));
					break;
				case P2P__IPC_MESSAGE__STATUS__BLOCK:
					printf("BLOCK ACCEPTED\n");
					break;
				case P2P__IPC_MESSAGE__STATUS__BLOCK_NOT_READY:
					printf("BLOCK IS NOT READY!\n");
					break;
				default:
					break;
					
			}
			break;
		case ASK_FOR_A_BLOCK:
			cl_p2p_send_STATUS(&ring,s,P2P__IPC_MESSAGE__STATUS__ASK_IF_BLOCK_READY, buffer_send);
        		if (p2p_logging_enabled) zlog_info(p2p_log, "ask for a block");
			printf("client send ask for a block\n");
    			io_uring_sqe_set_data64(sqe, make_request_data(s, WAITING));
			break;
		default: 
			break;
	}
    	io_uring_cqe_seen(&ring, cqe);
	cycle++;
    }
    io_uring_queue_exit(&ring);
    close(s);
    free(buffer);
    free(buffer_send);
    thrd_exit(1);
}


void cl_p2p_send_STATUS(struct io_uring *ring, int client_fd, P2pIpcMessage__Status STATUS,char* buffer_write  )
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
   //                                                   IPC_MESSAGE__STATUS__ASK_NEED_MSG); // write to client buffer
    size_t wr_len =  P2P_send_status(STATUS,buffer_write);
    io_uring_prep_send(sqe, client_fd, buffer_write, wr_len,
                       MSG_DONTWAIT);                                          // send a message
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
