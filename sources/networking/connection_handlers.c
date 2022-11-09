#include "connection_handlers.h"

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
    if (server_logging_enabled) zlog_info(server_log, "asking whether client need a msg");
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


void calc_merkle_tree(signed_message_t** msg_pointer){
    //-----create basic structures	
    unsigned long long EXPONENT = 9;
    unsigned long long n_msg = (1LLU << EXPONENT); //  create 2^9 messages
    layer_hp *L_arrays[EXPONENT];
    layer_hp L_arrays_p[EXPONENT]; // for free
    printf("number of messages in Merkle tree :%llu\n",n_msg);

    //user_keys uk = create_key_pair();
    signed_message_t** msg_arr = calloc(n_msg, sizeof(signed_message_t *));
    //fill messages to array
    size_t i = 0;
    for (i = 0; i < n_msg; i++)
    {
        msg_arr[i] = msg_pointer[i]; // pointer to message
        //validate_a_message(*msg_arr[i], uk.pk); debug
    }
    //-----------------------------------
    // CREATE BASE LAYER
    L_arrays[EXPONENT-1] = process_s_messagesV2(n_msg, msg_arr);
    L_arrays_p[EXPONENT - 1] = *L_arrays[EXPONENT - 1]; // store pointer
    n_msg >>= 1; // devide by 2
    //--------------------------
    // create intermedeate layers
    fill_intermediate_levels(EXPONENT, &n_msg, L_arrays, L_arrays_p); // done
    printf("Merkle root:\n");
    DumpHex( (*(L_arrays[0]->main_pointer))->hash  , crypto_generichash_BYTES);
    for (size_t i = 0; i < EXPONENT; i++)
    {
        destoroy_a_layer(L_arrays[i]);
    }
    free(msg_arr); // free conrainer for messages
}


void FINISH_SENDING(struct io_uring *ring, int client_fd)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    IpcMessage *ipc_msg = get_ipc_msg_buffer(client_fd);
    CBUF.fill_size = 0; // reset
    beffer_sended_N[client_fd] = 0; // reset
    if (server_logging_enabled) zlog_info(server_log,"calcin merkle root");
    calc_merkle_tree(CBUF.buffer); 

    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, FLAG_CLOSE_CONNECTION)); // set wait state
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
}



// send serialized dta and wait ackn
void handle_response_NEED_MORE_MSG(struct io_uring *ring, int client_fd)
{
    if (server_logging_enabled) zlog_info(server_log,"SENDED TO CURRENT CLIENT %lu", beffer_sended_N[client_fd]);
    if (beffer_sended_N[client_fd] >= 512 )
    { // block filled
               request_SEND_STATUS(ring, client_fd, IPC_MESSAGE__STATUS__ALL_BLOCK_MSG_SENDED);
    	       if (server_logging_enabled) zlog_info(server_log,"server sent all messages!!!!!!!!!!!!!!"); 
        return;
    }
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);                // add to ring
    memset(get_client_buffer(client_fd), 0, BUFFER_SIZE);             // set current buffer to zero;
    buffer_lengths[client_fd] = 0;                                    // set length to zero
    // validate here!
    signed_message_t* a_msg_p = GET_nth_circ_buf(&CBUF,beffer_sended_N[client_fd]);//send form buffer
    size_t n = serialize_data_v2(get_client_buffer(client_fd), a_msg_p,
                                 get_ipc_msg_buffer(client_fd)); // write serialized data to buf;
    buffer_lengths[client_fd] = n;
//     DumpHex(get_client_buffer(client_fd), n);
    io_uring_prep_send(sqe, client_fd, get_client_buffer(client_fd), n, MSG_CONFIRM); // read answer
    if (server_logging_enabled) zlog_info(server_log,"send signed message size = %zu serial: %lu ", buffer_lengths[client_fd],beffer_sended_N[client_fd]);
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, READ_RESPONSE));
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    // free(msg);
    beffer_sended_N[client_fd] += 1; // add sended
}

void READ_STATUS_RESPONSE(struct io_uring *ring, int client_fd)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    buffer_lengths[client_fd] = 0;                     // set length to zero
    io_uring_prep_recv(sqe, client_fd, get_client_buffer(client_fd), BUFFER_SIZE, 0);
    io_uring_sqe_set_data64(sqe, make_request_data(client_fd, TEST_RESPONSE));
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
}

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


