#include "setup_client.h"
#include <liburing.h>

void DumpHex(const void *data, size_t size);

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
  
  struct io_uring ring;
  io_uring_queue_init(10,&ring,0);


  struct io_uring_sqe* sqe = io_uring_get_sqe(&ring);
	
  io_uring_prep_connect(sqe, s, res->ai_addr, res->ai_addrlen);
  io_uring_submit(&ring);

  struct io_uring_cqe* cqe;
  io_uring_wait_cqe(&ring,&cqe);

  io_uring_cqe_seen(&ring,cqe);
  int ret = cqe->res;

// 	io_uring_for_each_cqe(ring, head, cqe)
  	
  char* buffer[1024];
  sqe = io_uring_get_sqe(&ring);
  io_uring_prep_recv(sqe,s,buffer,sizeof(buffer),0);
  io_uring_submit(&ring);
  io_uring_wait_cqe(&ring,&cqe);
  printf("readed %d\n",cqe->res);
  io_uring_cqe_seen(&ring,cqe);

  DumpHex(buffer,50);

  io_uring_queue_exit(&ring);
  printf("%d\n",ret);
  close(s);
  /*
  printf("setting up a socket for the client of %d\n",s);
	 return s;
  int c_done = connect(s, res->ai_addr, res->ai_addrlen);

  if (c_done < 0) {
    perror("error connecting to socket\n");
    exit(1);
  }
  */
  printf("connection...\n");

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
