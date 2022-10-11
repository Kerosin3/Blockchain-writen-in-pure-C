#include "connection_handlers.h"
/*#include "ipcmessages.h"
#include "misc.h"
#include "settings.h"
#include "setup.h"
#include <asm-generic/socket.h>
#include <bits/types/struct_timeval.h>
#include <stdlib.h>
#include <string.h>
*/
#define SOCKBUFSIZE 65536

int request_data_client_fd(uint64_t request_data)
{
    // UNIT_MAX = 0x00000000FFFFFFFF
    return request_data & UINT_MAX;
}

flag_state request_data_event_type(uint64_t request_data)
{
    uint64_t mask = ULLONG_MAX - UINT_MAX;
    // ULLONG_MAX - UINT_MAX = 0xFFFFFFFF00000000
    uint64_t ret = (request_data & (mask)) >> 32;
    return (flag_state)ret;
}

char *get_client_buffer(int client_fd)
{
    return &buffers[client_fd * BUFFER_SIZE];
}

IpcMessage *get_ipc_msg_buffer(int client_fd)
{
    return &buffer_transactions[client_fd * sizeof(IpcMessage)];
}

signed_message_t *get_signed_message_buffer(int client_fd)
{
    return &buffer_signed_message[client_fd * sizeof(signed_message_t)];
}

void set_flags(int socket)
{
    int sndsize = SOCKBUFSIZE;
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
}
// send with flag ask for transactions
void request_ASK_NEED_MSG(struct io_uring *ring, int client_fd)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    IpcMessage *ipc_msg = get_ipc_msg_buffer(client_fd);
    buffer_lengths[client_fd] = send_ONLY_status_code(ipc_msg, get_client_buffer(client_fd),
                                                      IPC_MESSAGE__STATUS__ASK_NEED_MSG); // write to client buffer
    io_uring_prep_send(sqe, client_fd, get_client_buffer(client_fd), buffer_lengths[client_fd],
                       MSG_DONTWAIT);                                          // send a message
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, READ_RESPONSE)); // set wait state
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
}

void request_SEND_STATUS(struct io_uring *ring, int client_fd, IpcMessage__Status status)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    IpcMessage *ipc_msg = get_ipc_msg_buffer(client_fd);
    buffer_lengths[client_fd] =
        send_ONLY_status_code(ipc_msg, get_client_buffer(client_fd), status); // write to client buffer
    io_uring_prep_send(sqe, client_fd, get_client_buffer(client_fd), buffer_lengths[client_fd],
                       MSG_DONTWAIT);                                          // send a message
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, READ_RESPONSE)); // set wait state
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
}

// send serialized dta and wait ackn
void handle_response_NEED_MORE_MSG(struct io_uring *ring, int client_fd)
{
    printf("SENDED TO CURRENT CLIENT %lu\n", beffer_sended_N[client_fd]);
    if (beffer_sended_N[client_fd] >= (1LU << 9LU))
    { // block filled
        request_SEND_STATUS(ring, client_fd, IPC_MESSAGE__STATUS__ALL_BLOCK_MSG_SENDED);
        return;
    }
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);                // add to ring
    memset(get_client_buffer(client_fd), 0, BUFFER_SIZE);             // set current buffer to zero;
    buffer_lengths[client_fd] = 0;                                    // set length to zero
    signed_message_t *a_msg_p = get_signed_message_buffer(client_fd); // write to client buffer
    user_keys uk = create_key_pair();
    a_msg_p = ls_get_a_signed_msg(uk); // generate random
    // validate here!
    size_t n = serialize_data_v2(get_client_buffer(client_fd), a_msg_p,
                                 get_ipc_msg_buffer(client_fd)); // write serialized data to buf;
    buffer_lengths[client_fd] = n;
    printf("SERDATA:%zu\n", n);
    DumpHex(get_client_buffer(client_fd), n);
    io_uring_prep_send(sqe, client_fd, get_client_buffer(client_fd), n, MSG_CONFIRM); // read answer
    printf("SIGNED message sended! size = %zu \n", buffer_lengths[client_fd]);
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, READ_RESPONSE));
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    // free(msg);
    beffer_sended_N[client_fd] += 1; // add sended
    destroy_signed_message(a_msg_p); // OK????
}

/*
void handle_response_IFNEED_MESSAGE(struct io_uring *ring, int client_fd)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    memset(get_client_buffer(client_fd),0,BUFFER_SIZE); // set current buffer to zero;
    buffer_lengths[client_fd] = 0; // set length to zero
    io_uring_prep_recv(sqe, client_fd, get_client_buffer(client_fd) , BUFFER_SIZE , 0);// read answer
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, FLAG_READ));
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
}
*/

/*
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // get entity
    io_uring_prep_recv(sqe, client_fd, get_client_buffer(client_fd) , BUFFER_SIZE ,0);// recv data
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, FLAG_READ));
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
}
*/

void READ_STATUS_RESPONSE(struct io_uring *ring, int client_fd)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    buffer_lengths[client_fd] = 0;                     // set length to zero
    io_uring_prep_recv(sqe, client_fd, get_client_buffer(client_fd), BUFFER_SIZE, 0);
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, TEST_RESPONSE));
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
}

/*
void add_read_request(struct io_uring *ring, int client_fd)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    size_t current_length = buffer_lengths[client_fd]; // get current length
    io_uring_prep_recv(sqe, client_fd, get_client_buffer(client_fd) + current_length, BUFFER_SIZE - current_length, 0);
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, FLAG_READ));
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
}
*/

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
}

void add_close_request(struct io_uring *ring, int client_fd)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); //
    if (!sqe)
    {
        printf("error adding to que\n");
    }
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, FLAG_READ));
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    printf("set state read\n");
}

void add_write_request_transaqtion(struct io_uring *ring, int client_fd, size_t nbytes, bool more_data)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
    io_uring_prep_send(sqe, client_fd, get_client_buffer(client_fd), nbytes, MSG_DONTWAIT | (more_data ? MSG_MORE : 0));
    io_uring_sqe_set_data(sqe, (void *)make_request_data(client_fd, FLAG_SEND_TRANSACTIONS));
    io_uring_submit(ring);
}

void handle_request_transactions(struct io_uring *ring, int client_fd)
{
    DumpHex(get_client_buffer(client_fd), 100);
    int n = snprintf(get_client_buffer(client_fd), BUFFER_SIZE, "hahahaha");
    buffer_lengths[client_fd] = n;
    add_write_request_transaqtion(ring, client_fd, n, false);
}
//  ---flag---clientfd

u_int64_t make_request_data(int client_fd, flag_state flag)
{
    return (uint64_t)(flag) << 32 | client_fd;
}

void add_write_request(struct io_uring *ring, int client_fd, size_t nbytes, bool more_data)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
    io_uring_prep_send(sqe, client_fd, get_client_buffer(client_fd), nbytes, MSG_DONTWAIT | (more_data ? MSG_MORE : 0));
    io_uring_sqe_set_data(sqe, (void *)make_request_data(client_fd, FLAG_WRITE));
    io_uring_submit(ring);
}

/*
void handle_request(struct io_uring *ring, int client_fd, size_t n_read)
{
    // size_t prev_length = buffer_lengths[client_fd];
    // size_t length = (buffer_lengths[client_fd] += n_read);        // add to length
    char *requested = extract_bytes(get_client_buffer(client_fd)); // read request
    int flag_found = 0;
    size_t k = 0;
    if (!requested)
    {
        printf("error precessing GET request\n");
        return;
    }
    while ((files_in_dir[k])) // check files in dir
    {
        if (!(strcmp(requested, files_in_dir[k])))
        {
            flag_found = 1;
            break;
        }
        k++;
    }
    if (flag_found)
    {
        int fds = open(files_in_dir[k],  O_SYNC | O_RDONLY); // open file
        if (fds < 0)
        {
            if (errno == EACCES) // access denied
            {
                int n = snprintf(get_client_buffer(client_fd), BUFFER_SIZE, "%s", http_403_content);
                buffer_lengths[client_fd] = n;
                file_fds[client_fd] = -1; // write -1
                add_write_request(ring, client_fd, n, false);
                free(requested);
                return;
            }
        }
        // send file
        file_fds[client_fd] = fds; // write req fd to client fd array

        struct stat st;
        stat(files_in_dir[k], &st);
        size_t csize = st.st_size;

        buffer_lengths[client_fd] = csize;

        int n = snprintf(get_client_buffer(client_fd), BUFFER_SIZE, REPLY_200OK, csize );
        add_write_request(ring, client_fd, n, true);
    }
    else if ((strcmp(requested, "main"))) // not main
    {
        int n = snprintf(get_client_buffer(client_fd), BUFFER_SIZE, "%s", http_404_content);
        buffer_lengths[client_fd] = n;
        file_fds[client_fd] = -1; // write -1
        add_write_request(ring, client_fd, n, false);
    }
    else // main page
    {
        int n = snprintf(get_client_buffer(client_fd), BUFFER_SIZE, main_page2, filesinthedir);
        buffer_lengths[client_fd] = n;
        file_fds[client_fd] = -1; // write -1
        add_write_request(ring, client_fd, n, false);
    }
    free(requested);
}
*/
