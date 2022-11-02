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
    add_accept_request(ring, sockfd, &client_addr, &client_addr_len);
    if (p2p_logging_enabled) zlog_info(p2p_log, "logging p2p application");
    int flag_block_ready = 0;
    int flag_client_connected = 0;
    add_accept_request(ring, sockfd, &client_addr, &client_addr_len);
    struct timespec mtimeout;
    mtimeout.tv_nsec = 100;
    mtimeout.tv_sec = 15;
	if (mtx_timedlock(&peer_connection_accepted, &mtimeout) ==  thrd_success){
		flag_client_connected = 1;
		mtx_unlock(&peer_connection_accepted);
	} else {
		printf("cannot wait till client started\n");
	}
	
    for (;;)
    {
	struct io_uring_cqe *cqe;
        if (UNLIKELY(io_uring_wait_cqe(ring, &cqe)))
            die("error accepting a connection\n");
	if (mtx_trylock(&block_created_mtx) == thrd_success) { // test if thread created block!
// 		memcpy(buffer_BLOCK_DATA, )
		flag_block_ready = 1;
		mtx_unlock(&block_created_mtx);
	}else {
		flag_block_ready = 0;
	}

        if (kill_thread_p2p) break;
        switch (request_data_event_type(cqe->user_data))
	{ 
	    case FLAG_ACCEPT:
		    printf("accepted a connection from p2p peer to this p2p client!\n");
    		    if (p2p_logging_enabled) zlog_info(p2p_log, "accepted a connection");
            	    add_accept_request(ring, sockfd, &client_addr,
                               &client_addr_len); // add requst one more time  and set socket id
		    current_client_fd = request_data_client_fd(cqe->res); // get current fd
                    set_flags(cqe->res); // set flags for the socket OK??
//                    request_ASK_NEED_MSG(ring, cqe->res); // send request ask MSG
                    P2P_send_PING(ring,current_client_fd);
		     printf("ping sended\n");
    		    if (p2p_logging_enabled) zlog_info(p2p_log, "ping sended");
   		     break;
	    case FLAG_WAIT_PONG:
		     P2P_read_status_response(ring, request_data_client_fd(cqe->user_data)); // recv
    		     if (p2p_logging_enabled) zlog_info(p2p_log, "testing response");
		     break;
	    case FLAG_TEST_RESPONSE:
		     if ((cqe->res) > 0){
		     	     P2pIpcMessage__Status msg_status;
			     msg_status = P2P_deserialize_STATUS( get_client_buffer(cqe->user_data), cqe->res);
			     printf("DESER MSG STATUS:%d\n",msg_status);
			     switch (msg_status) {
			     	case P2P__IPC_MESSAGE__STATUS__PONG: // received PONG --> ask if block is ready
					printf("GOT PONG RESPONSE\n");
    		     			if (p2p_logging_enabled) zlog_info(p2p_log, "p2p got pong response");
					P2Pser_send_STATUS(ring, request_data_client_fd(cqe->user_data) ,
							P2P__IPC_MESSAGE__STATUS__OK,FLAG_READ_RESPONSE);
    		     			if (p2p_logging_enabled) zlog_info(p2p_log, "sent ask block ready");
					break;
				case P2P__IPC_MESSAGE__STATUS__ASK_IF_BLOCK_READY: // IM BEING ASKED
					printf("server being asked if I have a block ready\n");
    		     			if (p2p_logging_enabled) zlog_info(p2p_log, "asked if this client have block ready");
					if (flag_block_ready) {
						printf("block ready!\n");
						P2Pser_send_block(ring, request_data_client_fd(cqe->user_data) );
    		     				if (p2p_logging_enabled) zlog_info(p2p_log, "block sended!");
					} else{
    		     				if (p2p_logging_enabled) zlog_info(p2p_log, "block NOT ready!");
						P2Pser_send_STATUS(ring, request_data_client_fd(cqe->user_data) ,
							P2P__IPC_MESSAGE__STATUS__BLOCK_NOT_READY,FLAG_READ_RESPONSE );
					}
					break;
				default:
					break;
			     }
		     }
		     break;
	    case FLAG_READ_RESPONSE:
		     P2P_read_status_response(ring, request_data_client_fd(cqe->user_data)); // recv
    		     if (p2p_logging_enabled) zlog_info(p2p_log, "reading response");
		     break;
	    case FLAG_BLOCK_SENDED:
			printf("block has been sent!\n");
		     break;
		default:
		     break;
	
	}
        io_uring_cqe_seen(ring, cqe);
	}

    if (p2p_logging_enabled) zlog_info(p2p_log, "exiting main p2p process!");
    printf("exiting p2p process\n");
    io_uring_queue_exit(ring);
    thrd_exit(1);
}
