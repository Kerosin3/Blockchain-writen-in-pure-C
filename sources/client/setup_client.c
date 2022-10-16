#include "setup_client.h"

#define BUFSIZE 4096
#define BLOCKSIZE 512
#define BUFSIZEFORMESSAGE 512
//size_t send_need_more_msg(struct io_uring *ring,int sock,void* buffer_wr);

//size_t send_STATUS(struct io_uring *ring,int sock,void* buffer_wr, IpcMessage__Status status_msg);

//size_t send_ACKN_OK(struct io_uring *ring,int sock,void* buffer_wr);
IpcMessage* buffer_transactions;


void teardown_server_sock(int sock)
{
    shutdown(sock, SHUT_RDWR);
}

int setup_client_iouring(){
  struct addrinfo hints, *res, *p;
  int status;
  char ipstr[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo("127.0.0.1", "12345", &hints, &res))) {
    printf("getaddrinfo error:%s\n", gai_strerror(status));
    exit(1);
  }
  for (p = res; p != NULL; p = p->ai_next) {
    void *addr;

    if (p->ai_family == AF_INET) {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
    } else {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
    }
    inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
  }

  int s = socket(res->ai_family, res->ai_socktype,
                 res->ai_protocol); // get a socket
  if (s==-1){
	  printf("cannot setup a client socket\n");
	  return -1;
  }
  
  buffer_transactions = calloc(1, sizeof(IpcMessage)); // buffers for storing current buffer size

  struct io_uring ring;
  io_uring_queue_init(10,&ring,0);


  struct io_uring_sqe* sqe = io_uring_get_sqe(&ring);
	
  io_uring_prep_connect(sqe, s, res->ai_addr, res->ai_addrlen); // prep connect
  printf("-->%d\n",s);

  if (io_uring_submit(&ring) < 0) // submit now!
        printf("error submitting\n");
  struct io_uring_cqe* cqe_main;
  io_uring_wait_cqe(&ring, &cqe_main);
  int ret = cqe_main->res;
  if (!ret) {
	  printf("connection established!\n");
  	  io_uring_cqe_seen(&ring,cqe_main);
  }
  printf("main socket: %d\n",s);
  IpcMessage__Status FLAG_FROM_SERVER = IPC_MESSAGE__STATUS__ERROR; // set error default status
  size_t k = 0;
  int flag_block_filled = 0;
  size_t count = 0;
  char* buffer = calloc(BUFSIZE, sizeof(char));
  char* buffer2 = calloc(BUFSIZE, sizeof(char));

  signed_message_t* msg_arr = calloc(BLOCKSIZE, sizeof(signed_message_t));
  for (size_t i = 0; i<BLOCKSIZE;i++) {
  	msg_arr[i].message = (unsigned char*) calloc(BUFSIZEFORMESSAGE,sizeof(char));// asign pointer
  }
  
  int ifread = 0;
  for(;;){
  	struct io_uring_cqe* cqe;
	if (flag_block_filled) {
		printf("block aquired!\n");
		break;
	}
	if (ifread == 0){
        sqe = io_uring_get_sqe(&ring); // return io entity
	io_uring_prep_recv(sqe,s,buffer,BUFSIZE*sizeof(char),0); // recv data
        io_uring_submit(&ring);
	io_uring_wait_cqe(&ring,&cqe);
  	io_uring_cqe_seen(&ring,cqe);
	ifread = 1;
	printf("--->\n");
	continue;
	}
	ifread = 0;
	int ret = cqe->res; // N readed bytes
// 	printf("readed %d\n",cqe->res);
	switch ( FLAG_FROM_SERVER = read_response_ONLY_STATUS(buffer, cqe->res)) {
		case IPC_MESSAGE__STATUS__ASK_NEED_MSG:
			printf("ASKED IF NEED MESSAGE:%d\n",FLAG_FROM_SERVER);	
			ret = send_STATUS(&ring,s,buffer2,IPC_MESSAGE__STATUS__NEED_MORE);
			break;
		case IPC_MESSAGE__STATUS__OK:
			printf("got message! %d \n",cqe->res);
			send_need_more_msg(&ring,cqe->res,buffer);
			break;
		case IPC_MESSAGE__STATUS__MESSAGE_SENDED:
			if (count < BLOCKSIZE-1){
			printf("got signed message!n =%lu \n",count);
	                DumpHex(buffer, cqe->res);

			printf("----%zu-----\n",count);
			deserialize_data_from_server(buffer,cqe->res,msg_arr+count);
			count++;
			//ret = send_ACKN_OK(&ring,s,buffer);
			ret = send_STATUS(&ring,s,buffer,IPC_MESSAGE__STATUS__ACKN_OK);
			} else if (count == BLOCKSIZE-1) {
				printf("--------count is %zu--------,readed %lu\n",count,cqe->res);
	                	DumpHex(buffer, cqe->res);
				deserialize_data_from_server(buffer,cqe->res,msg_arr+count);
				printf("STOP ACCEPTING\n");
				ret = send_STATUS(&ring,s,buffer2,IPC_MESSAGE__STATUS__ENOUGH); // 512 blocks acquired
				flag_block_filled = 1;
			} else {
				ret = send_STATUS(&ring,s,buffer2,IPC_MESSAGE__STATUS__ERROR); // 512 blocks acquired

			}
			break;
		case IPC_MESSAGE__STATUS__FINISH:
			printf("STOP ACCEPTING\n");
			break;
		case IPC_MESSAGE__STATUS__ALL_BLOCK_MSG_SENDED:
			printf("ALL BLOCKS ARE AQUIRED,exiting\n");
			ret = send_STATUS(&ring,s,buffer2,IPC_MESSAGE__STATUS__ALL_BLOCK_RECEIVED);
			flag_block_filled = 1;
			break;
		default:
			printf("default!\n");

			
	}

  	io_uring_cqe_seen(&ring,cqe);
	k++;
  }
  io_uring_queue_exit(&ring);
  free(buffer);
  free(buffer2);
  teardown_server_sock(s);
  close(s);
  free(buffer_transactions);
  freeaddrinfo(res);


//  calc_merkle_tree(msg_arr); 
//DumpHex(msg_arr[0].message,msg_arr[0].length);

  for (size_t i = 0 ; i<BLOCKSIZE; i++) {
	  free(msg_arr[i].message);
  }
  free(msg_arr);
}
