#include "setup_client.h"
#include "ipc_messages_client.h"
#include <liburing.h>

#define BUFSIZE 4096
void DumpHex(const void *data, size_t size) ;

size_t send_need_more_msg(struct io_uring *ring,int sock,void* buffer_wr);

size_t send_STATUS(struct io_uring *ring,int sock,void* buffer_wr, IpcMessage__Status status_msg);

size_t send_ACKN_OK(struct io_uring *ring,int sock,void* buffer_wr);
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
	  printf("cennot setup a client socket\n");
	  return -1;
  }
  
  buffer_transactions = calloc(1, sizeof(IpcMessage)); // buffers for storing current buffer size

  struct io_uring ring;
  io_uring_queue_init(10,&ring,0);


  struct io_uring_sqe* sqe = io_uring_get_sqe(&ring);
	
  io_uring_prep_connect(sqe, s, res->ai_addr, res->ai_addrlen); // prep connect

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
	printf("cycle begin\n");
	//io_uring_wait_cqe(&ring, &cqe); // return completion result
	int ret = cqe->res; // N readed bytes
	printf("readed %d\n",cqe->res);
	printf("readed buffer:\n");
	DumpHex(buffer, cqe->res);
	switch ( FLAG_FROM_SERVER = read_response_ONLY_STATUS(buffer, cqe->res)) {
		case IPC_MESSAGE__STATUS__ASK_NEED_MSG:
			printf("ASKED IF NEED MESSAGE:%d\n",FLAG_FROM_SERVER);	
		//	memset(buffer2,0,cqe->res); //set 0 ?????????????????????
			//ret = send_need_more_msg(&ring,s,buffer); // SEND NEED
			ret = send_STATUS(&ring,s,buffer2,IPC_MESSAGE__STATUS__NEED_MORE);
//			cqe->user_data = 1;
			printf("send need response\n");
			break;
		case IPC_MESSAGE__STATUS__OK:
			printf("got message! %d \n",cqe->res);
			DumpHex(buffer, cqe->res); // print out response
			//printf("sended ACK\n");
			send_need_more_msg(&ring,cqe->res,buffer);
			break;
		case IPC_MESSAGE__STATUS__MESSAGE_SENDED:
			printf("got signed message!\n");
			if (count!=511){
			printf("----%zu-----\n",count);
			count++;
			//ret = send_ACKN_OK(&ring,s,buffer);
			ret = send_STATUS(&ring,s,buffer,IPC_MESSAGE__STATUS__ACKN_OK);
			} else {
				printf("--------count is %zu--------\n",count);
				printf("STOP ACCEPTING\n");
				ret = send_STATUS(&ring,s,buffer2,IPC_MESSAGE__STATUS__ENOUGH); // 512 blocks acquired
				flag_block_filled = 1;
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

	//io_uring_wait_cqe(&ring, &cqe); // return completion result
	//memset(buffer,0,ret); //set 0 
	printf("cycle!\n");
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
}

size_t send_need_more_msg(struct io_uring *ring,int sock,void* buffer_wr)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    size_t n = send_ONLY_status_code(buffer_transactions,buffer_wr,IPC_MESSAGE__STATUS__NEED_MORE);
    io_uring_prep_send(sqe, sock, buffer_wr , n , MSG_DONTWAIT);// read answer
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    return n;


}

size_t send_ACKN_OK(struct io_uring *ring,int sock,void* buffer_wr){
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    size_t n = send_ONLY_status_code(buffer_transactions,buffer_wr,IPC_MESSAGE__STATUS__ACKN_OK);
    io_uring_prep_send(sqe, sock, buffer_wr , n , MSG_DONTWAIT);// read answer
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    return n;
}


size_t send_STATUS(struct io_uring *ring,int sock,void* buffer_wr, IpcMessage__Status status_msg){
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    size_t n = send_ONLY_status_code(buffer_transactions,buffer_wr,status_msg);
    printf("sending status code %d\n",status_msg);
    DumpHex(buffer_wr, n);
    io_uring_prep_send(sqe, sock, buffer_wr , n , MSG_DONTWAIT);// read answer
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    return n;
}



/*
 * for debugging
 * */
void DumpHex(const void *data, size_t size) {
  char ascii[17];
  size_t i, j;
  ascii[16] = '\0';
  for (i = 0; i < size; ++i) {
    printf("%02X ", ((unsigned char *)data)[i]);
    if (((unsigned char *)data)[i] >= ' ' &&
        ((unsigned char *)data)[i] <= '~') {
      ascii[i % 16] = ((unsigned char *)data)[i];
    } else {
      ascii[i % 16] = '.';
    }
    if ((i + 1) % 8 == 0 || i + 1 == size) {
      printf(" ");
      if ((i + 1) % 16 == 0) {
        printf("|  %s \n", ascii);
      } else if (i + 1 == size) {
        ascii[(i + 1) % 16] = '\0';
        if ((i + 1) % 16 <= 8) {
          printf(" ");
        }
        for (j = (i + 1) % 16; j < 16; ++j) {
          printf("   ");
        }
        printf("|  %s \n", ascii);
      }
    }
  }
}