#include "p2p_event_loop.h"



void event_loop_p2p(event_p2p_params_t* elparams)
{

    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_len = sizeof(client_addr);
    int current_client_fd = -1;
    int sndsize = 65536;
    int err;
    int sockfd = elparams->serv_sock;
    struct io_uring* ring = elparams->ring;
    if ((err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_SNDBUF, (char *)&sndsize, (int)sizeof(sndsize))))
        strerror(err);
    if ((err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_RCVBUF, (char *)&sndsize, (int)sizeof(sndsize))))
        strerror(err);
    unsigned long long ii = 0;
//    add_accept_request(ring, sockfd, &client_addr, &client_addr_len);
    if (p2p_logging_enabled) zlog_info(p2p_log, "logging p2p application");

    add_accept_request(ring, sockfd, &client_addr, &client_addr_len);
    for (;;)
    {
	struct io_uring_cqe *cqe;
        if (UNLIKELY(io_uring_wait_cqe(ring, &cqe)))
            die("error accepting a connection\n");
	if (flag_block_created) { // test if thread created block!
		printf("BLOCK CREATED!\n");

	}else {
		printf("block not created!\n");
	}

        if (kill_thread_p2p) break;
        switch (request_data_event_type(cqe->user_data))
	{ 
	    case FLAG_ACCEPT:
		    printf("accepted a connection!\n");
		     break;
		default:
		     break;
	
	}
        io_uring_cqe_seen(ring, cqe);
	}

    if (p2p_logging_enabled) zlog_info(p2p_log, "exiting main p2p process!");
    printf("exiting p2p process\n");
    io_uring_queue_exit(ring);
}