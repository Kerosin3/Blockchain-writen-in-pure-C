#include "start_server.h"

struct io_uring ring;

void start_server(uint16_t port)
{

    setup_buffers(MAX_CONNECTIONS);      // establish buffers
    int serv_fd = setup_serv_sock(port); // set server fd
    printf(">>launching server on a port: %d<<\n", port);
    setup_iouring(&ring, MAX_CONNECTIONS, false);
    signal(SIGPIPE, SIG_IGN);

    event_loop(serv_fd, &ring);

    destroy_buffers();
}
