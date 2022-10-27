#include <p2p_listen.h>


#define ring_entries_num 10

void setup_p2p_listening(char* IP_ADD_LISTEN)
{
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo( IP_ADD_LISTEN,"10001", &hints, &res)))
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
    }else {
	    printf("established socket for incoming p2p connections!\n");
    }
/*
    struct io_uring ring;
    io_uring_queue_init(ring_entries_num, &ring, 0);

    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);

    io_uring_prep_connect(sqe, s, res->ai_addr, res->ai_addrlen); // prep connect

    if (io_uring_submit(&ring) < 0) // submit now!
        printf("error submitting\n");
    struct io_uring_cqe *cqe_main;
    io_uring_wait_cqe(&ring, &cqe_main);
    int ret = cqe_main->res;

    if (ret==0) //  что делать?????
    {
    //    zlog_info(client_log, "connection to server has been established!");
        printf("connection TO MESSAGE SERVER has been established!\n");
        io_uring_cqe_seen(&ring, cqe_main);
    } else {
      //  zlog_info(client_log, "error while trying connecting to the server");
	printf("error establishing connection to server, exiting\n");
	close(s);
	return;

    }*/
    int ifread = 0;
}

