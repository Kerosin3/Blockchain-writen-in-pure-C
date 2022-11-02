#include "dhandlers_p2p.h"
//P2P_read_status_response(ring, request_data_client_fd(cqe->user_data)); // recv
//    		     if (p2p_logging_enabled) zlog_info(p2p_log, "testing response");
//		     break;

int request_data_client_fd(uint64_t request_data)
{
    // UNIT_MAX = 0x00000000FFFFFFFF
    return request_data & UINT_MAX;
}

STATE_P2P request_data_event_type(uint64_t request_data)
{
    uint64_t mask = ULLONG_MAX - UINT_MAX;
    // ULLONG_MAX - UINT_MAX = 0xFFFFFFFF00000000
    uint64_t ret = (request_data & (mask)) >> 32;
    return (STATE_P2P)ret;
}

char *get_client_buffer(int client_fd)
{
    return &buffers[client_fd * BUFFER_SIZE];
}
//  ---flag---clientfd

u_int64_t make_request_data(int client_fd, STATE_P2P flag)
{
    return (uint64_t)(flag) << 32 | client_fd;
}

//-------------------------------------------------------//
void add_accept_request(struct io_uring *ring, int serverfd, struct sockaddr_in *a_client_adrd,
                        socklen_t *client_addr_len)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // pop
    if (!sqe)
    {
        printf("error adding to que\n");
    }
    io_uring_prep_accept(sqe, serverfd, (struct sockaddr *)a_client_adrd, client_addr_len, 0);
    io_uring_sqe_set_data64(sqe, make_request_data(0, FLAG_ACCEPT));
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    if (p2p_logging_enabled) zlog_info(p2p_log, "p2p set ACCEPT state");
}

void P2P_send_PING(struct io_uring *ring, int client_fd)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
//     IpcMessage *ipc_msg = get_ipc_msg_buffer(client_fd);
 //   buffer_lengths[client_fd] = send_ONLY_status_code(ipc_msg, get_client_buffer(client_fd),
   //                                                   IPC_MESSAGE__STATUS__ASK_NEED_MSG); // write to client buffer
    P2pIpcMessage__Status STATUS = P2P__IPC_MESSAGE__STATUS__PING;
    size_t wr_len =  P2P_send_status(STATUS,get_client_buffer(client_fd));
    buffer_lengths[client_fd] = wr_len;
    io_uring_prep_send(sqe, client_fd, get_client_buffer(client_fd), buffer_lengths[client_fd],
                       MSG_DONTWAIT);                                          // send a message
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, FLAG_WAIT_PONG)); // set wait state
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    if (p2p_logging_enabled) zlog_info(p2p_log, "sent PING msg");
}
void P2Pser_send_STATUS(struct io_uring *ring, int client_fd,  P2pIpcMessage__Status STATUS , STATE_P2P STATE )
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
//     IpcMessage *ipc_msg = get_ipc_msg_buffer(client_fd);
 //   buffer_lengths[client_fd] = send_ONLY_status_code(ipc_msg, get_client_buffer(client_fd),
   //                                                   IPC_MESSAGE__STATUS__ASK_NEED_MSG); // write to client buffer
    size_t wr_len =  P2P_send_status(STATUS,get_client_buffer(client_fd));
    buffer_lengths[client_fd] = wr_len;
    io_uring_prep_send(sqe, client_fd, get_client_buffer(client_fd), buffer_lengths[client_fd],
                       MSG_DONTWAIT);                                          // send a message
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, STATE)); // set wait state
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    if (p2p_logging_enabled) zlog_info(p2p_log, "sent msg %d",STATUS);
}

void P2Pser_send_block(struct io_uring *ring, int client_fd)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
//     IpcMessage *ipc_msg = get_ipc_msg_buffer(client_fd);
 //   buffer_lengths[client_fd] = send_ONLY_status_code(ipc_msg, get_client_buffer(client_fd),
   //                                                   IPC_MESSAGE__STATUS__ASK_NEED_MSG); // write to client buffer
    buffer_lengths[client_fd] = block_written_size;
    memcpy( get_client_buffer(client_fd),  buffer_BLOCK_DATA ,BUFFER_SIZE);
//     DumpHex(get_client_buffer(client_fd), block_written_size);
    io_uring_prep_send(sqe, client_fd, get_client_buffer(client_fd), buffer_lengths[client_fd],
                       MSG_DONTWAIT);                                          // send a message
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, FLAG_BLOCK_SENDED)); // set wait state
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    if (p2p_logging_enabled) zlog_info(p2p_log, "block sended!");
}



void P2P_read_status_response(struct io_uring *ring, int client_fd)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    buffer_lengths[client_fd] = 0;                     // set length to zero
    io_uring_prep_recv(sqe, client_fd, get_client_buffer(client_fd), BUFFER_SIZE, 0);
//     DumpHex(get_client_buffer(client_fd),wr_len);
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, FLAG_TEST_RESPONSE));
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
}


//--------------------------------------------//
//

void set_flags(int socket)
{
    size_t sndsize = 65536;
    int err;

    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    if ((err = setsockopt(socket, SOL_SOCKET, SO_REUSEADDR | SO_SNDBUF, (char *)&sndsize, (int)sizeof(sndsize))))
        strerror(err);
    if ((err = setsockopt(socket, SOL_SOCKET, SO_REUSEADDR | SO_RCVBUF, (char *)&sndsize, (int)sizeof(sndsize))))
        strerror(err);
    if ((err = setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout))) < 0)
        strerror(err);
    if ((err = setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout))) < 0)
        strerror(err);
    if ((err = setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0))
       strerror(err);
}
