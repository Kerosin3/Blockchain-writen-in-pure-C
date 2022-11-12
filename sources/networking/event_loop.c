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
    PUSH_msg_circ_buf(&CBUF); // create buffer
    add_accept_request(ring, sockfd, &client_addr, &client_addr_len);
    if (server_logging_enabled)
        zlog_info(server_log, "logging..");
    for (;;)
    {

        struct io_uring_cqe *cqe;
        if (UNLIKELY(io_uring_wait_cqe(ring, &cqe)))
            die("error accepting a connection");
        // printf("current fd is %d, cycle serial:%llu\n", cqe->res, ii);
        switch (request_data_event_type(cqe->user_data)) // get type EVENT
        {
        case FLAG_ACCEPT:
            if (server_logging_enabled)
                zlog_info(server_log, "server accept incoming client with socket %d", cqe->res);
            add_accept_request(ring, sockfd, &client_addr,
                               &client_addr_len); // add requst one more time  and set socket id

            current_client_fd = request_data_client_fd(cqe->res); // get current fd
            set_flags(cqe->res);                                  // set flags for the socket OK??
            request_ASK_NEED_MSG(ring, cqe->res);                 // send request ask MSG
            break;
        case READ_RESPONSE: // wait response
            if (LIKELY(cqe->res))
            {                                                                       // received some bytes
                READ_STATUS_RESPONSE(ring, request_data_client_fd(cqe->user_data)); // WILL BE WAIT TILL RECV WORKS
            }
            break;
        case WAIT_ACKNOWLEDGEMENT:
            if (server_logging_enabled)
                zlog_info(server_log, "waiting client responce");
            break;
        case FLAG_CLOSE_CONNECTION: // close connection for a client
            if (server_logging_enabled)
                zlog_info(server_log, "closing connection to the client %d", request_data_client_fd(cqe->user_data));
            shutdown(current_client_fd, SHUT_RDWR);
            int closeret = close(current_client_fd);
            if (closeret < 0)
            {
                if (server_logging_enabled)
                    zlog_info(server_log, "error closing socket %d, error %s", current_client_fd, strerror(errno));
            }
            beffer_sended_N[request_data_client_fd(cqe->user_data)] = 0; // add sended
            break;
        case TEST_RESPONSE:
            if ((cqe->res) > 0)
            { // non-empty request?  set fd test not zero read
                if (server_logging_enabled)
                    zlog_info(server_log, "testing client responce, readed %d bytes", cqe->res);
                IpcMessage__Status STATUS;
                switch (STATUS = read_ONLY_STATUS_response(get_client_buffer(cqe->user_data), cqe->res))
                {
                case (IPC_MESSAGE__STATUS__NEED_MORE):
                    if (server_logging_enabled)
                        zlog_info(server_log, "client needs more messages");
                    handle_response_NEED_MORE_MSG(ring, cqe->user_data);
                    break;
                case (IPC_MESSAGE__STATUS__ACKN_OK):
                    if (server_logging_enabled)
                        zlog_info(server_log, "client acknowledged the message");
                    // 					destroy_signed_message(get_signed_message_buffer(request_data_client_fd(cqe->user_data)));
                    request_ASK_NEED_MSG(ring, request_data_client_fd(cqe->user_data)); // send request ask MSG
                    if (server_logging_enabled)
                        zlog_info(server_log, "asking whether needs a message");
                    break;
                case (IPC_MESSAGE__STATUS__ENOUGH):
                    // 		    printf("client fd %zu \n", beffer_sended_N[ request_data_client_fd(cqe->res)  ] );
                    FINISH_SENDING(ring, request_data_client_fd(cqe->user_data));
                    if (server_logging_enabled)
                        zlog_info(server_log, "client got enough,(512 messages)");
                    fprintf(stdout, "client %d accepted 512 messages\nClosing connection\n",
                            request_data_client_fd(cqe->user_data));
                    break;
                case (IPC_MESSAGE__STATUS__ALL_BLOCK_RECEIVED):
                    break;
                default:
                    break;
                }
            }
            else
            {
                if (server_logging_enabled)
                    zlog_info(server_log, "error while processing %d", cqe->res);
            }
            break;
        case FLAG_SEND_TRANSACTIONS:
            if (server_logging_enabled)
                zlog_info(server_log, "server has sended message");
            break;
        default:
            break;
        }
        io_uring_cqe_seen(ring, cqe);
    }
    io_uring_queue_exit(ring);
}
