#include "p2p_event_loop.h"



void event_loop_p2p(int sockfd, struct io_uring *ring)
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
//    add_accept_request(ring, sockfd, &client_addr, &client_addr_len);
    if (client_logging_enabled) zlog_info(client_log, "logging p2p appication");
    for (;;)
    {
	    

	}
}
