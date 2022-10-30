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

    if (p2p_logging_enabled) zlog_info(p2p_log, "p2p client tryint to connect...");

    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
       strerror("setsockopt(SO_REUSEADDR) failed");
    int rezopt = fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK);
    if (rezopt == -1){
     perror("calling fcntl");
    }
    if (p2p_logging_enabled) zlog_info(p2p_log, "setting socket nonblock...");

     int done = 1;
     while(done!=0){
	     done = connect(s,res->ai_addr,res->ai_addrlen);
	     printf("trying to connect againg\n");
	     sleep(2);
     }
    if (p2p_logging_enabled) zlog_info(p2p_log, "setting socket nonblock...");
    printf("GOT CONNECTION!\n");
    struct io_uring ring;
    io_uring_queue_init(ring_entries_num, &ring, 0);

     struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
     struct io_uring_cqe *cqe;

     if (io_uring_submit(&ring) < 0) // submit now!
         printf("error submitting\n");
/*
     while(!done)
     {
       	printf("waiting\n");
	int rez=0;
      	sqe = io_uring_get_sqe(&ring);
        rez = io_uring_wait_cqe(&ring, &cqe_main); // waiting
 	if (rez){
  //	if ((rez = io_uring_wait_cqe_timeout(&ring, &cqe_main,&ts))!=0){ // if timedout
		printf("bad code: %d\n",rez);
//  		continue;
	} else { // check result if not timed_outed
		printf("test cqe result\n");
    		int ret = cqe_main->res;//checkk after connection
		printf("ret is %d\n",ret);
    		if (ret==0) //  что делать????? ждать
    		{
	    //    zlog_info(client_log, "connection to server has been established!");
        	printf(">>>>>connection to p2p peer established!<<<<<<\n");
		done = 1;
//      	   io_uring_cqe_seen(&ring, cqe_main);
                io_uring_cqe_seen(&ring, cqe_main);
		break; // out cycle if connect is ok
    		}
	} 
//         io_uring_cqe_seen(&ring, cqe_main);
	printf("trying again\n");
     }
    //  zlog_info(client_log, "error while trying connecting to the server");
//     	io_uring_cqe_seen(&ring, cqe_main);
 	//printf("sleep 2 seconds, done =%d \n",done);
	//printf("seen!\n");
        //sleep(2);
 */ int ifread = 0;
    unsigned char buffer[4096] = {0};
    for (;;){
//	printf("in cycle!\n"); 
	if (ifread == 0)
        {
            sqe = io_uring_get_sqe(&ring);                                 // return io entity
            io_uring_prep_recv(sqe, s, buffer, 4096 * sizeof(char), 0); // recv data
            io_uring_submit(&ring);
            io_uring_wait_cqe(&ring, &cqe);
	    printf("received!\n");
            ifread = 0;
            //break;
        }
    io_uring_cqe_seen(&ring, cqe);
    }
    close(s);
}

