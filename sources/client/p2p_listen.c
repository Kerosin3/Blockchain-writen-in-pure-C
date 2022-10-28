#include <p2p_listen.h>
#include <linux/time_types.h>

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

    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
       strerror("setsockopt(SO_REUSEADDR) failed");
    struct io_uring ring;
    io_uring_queue_init(ring_entries_num, &ring, 0);

     struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
     int done = 0;
     struct __kernel_timespec ts;
        ts.tv_sec = 2LL; // wait 2 sec
        ts.tv_nsec = 1000;

     while(!done)
     {
     struct io_uring_cqe *cqe_main;
    	io_uring_prep_connect(sqe, s, res->ai_addr, res->ai_addrlen); // prep connect
//         if (io_uring_submit(&ring) < 0) // submit now!
//         printf("error submitting\n");
	printf("waiting\n");
	int rez=0;
     	sqe = io_uring_get_sqe(&ring);
//         rez = io_uring_wait_cqe(&ring, &cqe_main); // waiting
// 	if ((rez !=0)){ // if timedout
 	if ((rez = io_uring_wait_cqe_timeout(&ring, &cqe_main,&ts))!=0){ // if timedout
		printf("bad %d\n",rez);
//                 sleep(2);
//  		continue;
	} else { // check result if not timed_outed
		printf("finish\n");
    		int ret = cqe_main->res;//checkk after connection
		printf("ret is %d\n",ret);
    		if (ret==0) //  что делать????? ждать
    		{
	    //    zlog_info(client_log, "connection to server has been established!");
        	printf(">>>>>connection to p2p peer established!<<<<<<\n");
		done = 1;
//      	   io_uring_cqe_seen(&ring, cqe_main);
		break; // out cycle if connect is ok
    		}
	} 
        io_uring_cqe_seen(&ring, cqe_main);
	printf("trying again\n");
     }
    //  zlog_info(client_log, "error while trying connecting to the server");
//     	io_uring_cqe_seen(&ring, cqe_main);
 	//printf("sleep 2 seconds, done =%d \n",done);
	//printf("seen!\n");
        //sleep(2);
    int ifread = 0;
    unsigned char buffer[4096] = {0};
    for (;;){
        struct io_uring_cqe *cqe;
//	printf("in cycle!\n"); 
	if (ifread == 0)
        {
            sqe = io_uring_get_sqe(&ring);                                 // return io entity
            io_uring_prep_recv(sqe, s, buffer, 4096 * sizeof(char), 0); // recv data
            io_uring_submit(&ring);
            io_uring_wait_cqe(&ring, &cqe);
            io_uring_cqe_seen(&ring, cqe);
            ifread = 1;
            //break;
        }
    io_uring_cqe_seen(&ring, cqe);
    }
    close(s);
}

