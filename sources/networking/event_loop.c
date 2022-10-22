#include "event_loop.h"


void event_loop(int sockfd, struct io_uring *ring)
{

    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_len = sizeof(client_addr);
    int current_client_fd = -1;
    int sndsize = 65536;
    int err;
    if ((err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_SNDBUF, (char *)&sndsize, (int)sizeof(sndsize))))
        strerror(err);
    if ((err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_RCVBUF, (char *)&sndsize, (int)sizeof(sndsize))))
        strerror(err);
    unsigned long long ii = 0;
    PUSH_msg_circ_buf(&CBUF);
    add_accept_request(ring, sockfd, &client_addr, &client_addr_len);
    if (logging_enabled) zlog_info(server_log, "server started!");
    for (;;)
    {

        struct io_uring_cqe *cqe;
        if (UNLIKELY(io_uring_wait_cqe(ring, &cqe)))
            die("error accepting a connection");
        // printf("current fd is %d, cycle serial:%llu\n", cqe->res, ii);
        switch (request_data_event_type(cqe->user_data)) // get type EVENT
        {
        case FLAG_ACCEPT:
    	if (logging_enabled) zlog_info(server_log, "accepted a connection!, client socket %d",cqe->res);
            add_accept_request(ring, sockfd, &client_addr,
                               &client_addr_len); // add requst one more time  and set socket id

            current_client_fd = request_data_client_fd(cqe->res); // get current fd
            set_flags(cqe->res); // set flags for the socket OK??
    	    if (logging_enabled) zlog_info(server_log, "client asked if needs message");
            request_ASK_NEED_MSG(ring, cqe->res); // send request ask MSG
            break;
        case READ_RESPONSE: // wait response
    	    if (logging_enabled) zlog_info(server_log, "message been sended, size = %d",cqe->res);
            if (LIKELY(cqe->res))
            { // received some bytes
    	        if (logging_enabled) zlog_info(server_log, "waiting response from socket %llu",cqe->user_data);
                READ_STATUS_RESPONSE(ring, request_data_client_fd(cqe->user_data)); // WILL BE WAIT TILL RECV WORKS
            }
            break;
        case WAIT_ACKNOWLEDGEMENT:
    	    if (logging_enabled) zlog_info(server_log, "server waits acknowledgement");
            break;
        case FLAG_CLOSE_CONNECTION:
    	    if (logging_enabled) zlog_info(server_log, "closing connection to current client");
            shutdown(current_client_fd, SHUT_RDWR);
            int closeret = close(current_client_fd);
            if (closeret < 0)
    	    	if (logging_enabled) zlog_info(server_log, "error while closing socket %d, %s\n", current_client_fd, strerror(errno));
    		beffer_sended_N[request_data_client_fd(cqe->user_data)] =0; // set sended ti 0
            break;
        case TEST_RESPONSE:
            if ((cqe->res) > 0)
            { // non-empty request
    	    	if (logging_enabled) zlog_info(server_log, "testing client response, readed %d bytes",cqe->res);
                IpcMessage__Status STATUS;
                switch (STATUS = read_ONLY_STATUS_response(get_client_buffer(cqe->user_data), cqe->res))
                {
                case (IPC_MESSAGE__STATUS__NEED_MORE):
                    handle_response_NEED_MORE_MSG(ring, cqe->user_data);
                    break;
                case (IPC_MESSAGE__STATUS__ACKN_OK):
                    // destroy_signed_message(get_signed_message_buffer(request_data_client_fd(cqe->user_data)));
                    request_ASK_NEED_MSG(ring, request_data_client_fd(cqe->user_data)); // send request ask MSG
                    break;
                case (IPC_MESSAGE__STATUS__ENOUGH):
                    FINISH_SENDING(ring, request_data_client_fd(cqe->user_data));
    	    	    if (logging_enabled) zlog_info(server_log, "client said accepted all transactions ");
                    printf("WHOLE BLOCK SENDED\n");
                    break;
                case (IPC_MESSAGE__STATUS__ALL_BLOCK_RECEIVED):
                    printf("OK, WAITING......\n");
                    break;
                default:
                    break;
                }
            }
            else
            {
    	        if (logging_enabled) zlog_info(server_log, "error while processing socket %d",cqe->res);
            }
            break;
        }
        /* when??
            shutdown(current_client_fd, SHUT_RDWR);
            int closeret = close(current_client_fd);
            if (closeret < 0)
                   printf("error while closing socket %d, %s\n", current_client_fd, strerror(errno));

        */
        io_uring_cqe_seen(ring, cqe);
    }
}
