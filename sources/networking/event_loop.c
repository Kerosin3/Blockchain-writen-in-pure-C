#include "event_loop.h"
#include "connection_handlers.h"
#include "misc.h"
#include "setup.h"


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
    add_accept_request(ring, sockfd, &client_addr, &client_addr_len);
    for(;;){

        struct io_uring_cqe *cqe;
	if (UNLIKELY(io_uring_wait_cqe(ring, &cqe)))
            die("error accepting a connection");
        //printf("current fd is %d, cycle serial:%llu\n", cqe->res, ii);
        switch (request_data_event_type(cqe->user_data)) // get type EVENT
	{
		case FLAG_ACCEPT:
			printf("Flag accept!\n");
            		add_accept_request(ring, sockfd, &client_addr,
                               &client_addr_len); // add requst one more time  and set socket id
			printf("current client socket: %d \n",cqe->res);
	            	buffer_lengths[cqe->res] = 0;         // set current read buffer 0
        	        set_flags(cqe->res);                  // set flags for the socket
			add_ask_transaqtion_request(ring,cqe->res);
			break;
        	case WAIT_ANSWER_FOR_TRANSACTIONS:
			printf("ask for transaqtions\n");
			printf("sended %d bytes:\n",cqe->res);
	            if (LIKELY(cqe->res)) {// non-empty request?  set fd test not zero read
				handle_responce(ring, request_data_client_fd(cqe->user_data));
			}
			break;
		case FLAG_READ:
	            if (LIKELY(cqe->res)) {// non-empty request?  set fd test not zero read
			printf("reading responce\n");	
			printf("readed %d bytes:\n",cqe->res);
			DumpHex(get_client_buffer(cqe->user_data),cqe->res);

		    }
	                break;
	}
        io_uring_cqe_seen(ring, cqe);
    }
}



