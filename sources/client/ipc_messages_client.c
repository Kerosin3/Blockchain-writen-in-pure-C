#include "ipc_messages_client.h"
#include "p2ptransaction.pb-c.h"
#include <stdint.h>

/*
size_t get_timestamp(void* buffer){
    char date[32];
        time_t t = time(NULL);
        struct tm* tm = gmtime(&t);
        strftime(date, sizeof(date), "%d/%m/%Y %H:%M:%S GMT", tm);
    int n = snprintf(buffer, strlen(date)+1, "%s", date);
    return n;
}
*/

//--------------------------------client serdes to p2p--------------------------------------------------//

size_t P2P_serialize_block_to_sock(block_t* a_block,void* buf_out){
	P2pIpcMessage* ipc_msg_p2p = calloc(1,sizeof(P2pIpcMessage));
	p2p__ipc_message__init(ipc_msg_p2p);//init
	ipc_msg_p2p->has_difficulty = 1;			    //
	ipc_msg_p2p->difficulty = a_block->difficulty;
	ipc_msg_p2p->has_timestamp_end = 1;
	ipc_msg_p2p->has_timestamp_begin = 1;
	ipc_msg_p2p->timestamp_begin = a_block->timestamp_begin;
	ipc_msg_p2p->timestamp_end = a_block->timestamp_end;
	ipc_msg_p2p->has_merkle_root = 1;
	ipc_msg_p2p->merkle_root.data = a_block->merkle_root;
	ipc_msg_p2p->merkle_root.len = crypto_generichash_BYTES;
	ipc_msg_p2p->has_nonce = 1;
	ipc_msg_p2p->nonce.data = a_block->nonce;
	ipc_msg_p2p->nonce.len = 15;
	ipc_msg_p2p->has_prev_block = 1;
	ipc_msg_p2p->prev_block.data = a_block->previous_block;
	ipc_msg_p2p->prev_block.len = crypto_generichash_BYTES;
	//---------------date//
	ipc_msg_p2p->date = a_block->date; 
	ipc_msg_p2p->status_code = P2P__IPC_MESSAGE__STATUS__BLOCK;
	size_t len = p2p__ipc_message__get_packed_size(ipc_msg_p2p);
	p2p__ipc_message__pack(ipc_msg_p2p, buf_out);
	free(ipc_msg_p2p);// free memory
	return len;
}


size_t P2P_send_status(P2pIpcMessage__Status STATUS,void* buf_out){
	P2pIpcMessage* ipc_msg_p2p = calloc(1,sizeof(P2pIpcMessage));
	p2p__ipc_message__init(ipc_msg_p2p);//init
	ipc_msg_p2p->has_difficulty = 0;			    //
	ipc_msg_p2p->has_timestamp_end = 0;
	ipc_msg_p2p->has_timestamp_begin = 0;
	ipc_msg_p2p->has_merkle_root = 0;
	ipc_msg_p2p->has_nonce = 0;
	ipc_msg_p2p->has_prev_block = 0;
	//---------------date//
	char date[35] = {0};
	get_timestamp(date);
	ipc_msg_p2p->date = date; 
	ipc_msg_p2p->status_code = P2P__IPC_MESSAGE__STATUS__BLOCK;
	size_t len = p2p__ipc_message__get_packed_size(ipc_msg_p2p);
	p2p__ipc_message__pack(ipc_msg_p2p, buf_out);
	free(ipc_msg_p2p);// free memory
	return len;
}

P2pIpcMessage__Status P2P_deserialize_STATUS(char *buff, unsigned len){
    P2pIpcMessage *message;
    message = p2p__ipc_message__unpack(0, len,(uint8_t*) buff);
    P2pIpcMessage__Status status;	    
    p2p__ipc_message__free_unpacked(message, NULL);
    status = message->status_code;
    return status;
}


block_t* deserialize_block(void* buf_in,unsigned len){
	block_t* block = calloc(1,sizeof(block_t));
	P2pIpcMessage *message;
        message = p2p__ipc_message__unpack(0, len,(uint8_t*) buf_in);
	if ((message->status_code) != P2P__IPC_MESSAGE__STATUS__BLOCK) return 0;
	memcpy(block->date,message->date,35);
// 	memcpy(block->difficulty,message->difficulty,)
        memcpy(block->merkle_root,message->merkle_root.data,message->merkle_root.len);
	memcpy(block->nonce, message->nonce.data,message->nonce.len);

	block->timestamp_begin = message->timestamp_begin;
	block->timestamp_end = message->timestamp_end;
	memcpy(block->previous_block,message->prev_block.data,message->prev_block.len );
	block->difficulty = message->difficulty;
	return block;
} 








size_t send_need_more_msg(struct io_uring *ring, int sock, void *buffer_wr)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    size_t n = send_ONLY_status_code(buffer_transactions, buffer_wr, IPC_MESSAGE__STATUS__NEED_MORE);
    io_uring_prep_send(sqe, sock, buffer_wr, n, MSG_DONTWAIT); // read answer
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    return n;
}
//-------------------------------------------------------------------------------------------------------//
void deserialize_data_from_server(char *buff, unsigned len, signed_message_t *msg)
{
    IpcMessage *message;
    message = ipc_message__unpack(0, len,(uint8_t*) buff);
    if (message->has_pubkey)
    {
        memcpy(msg->public_key, message->pubkey.data, crypto_sign_PUBLICKEYBYTES); // copy public key
    }
    if (message->has_transaction_msg)
    {
        msg->length = message->transaction_msg.len;               // assign length
        memcpy(msg->message, message->transaction_msg.data, message->transaction_msg.len); // copy data to pointer
                                                                  // DumpHex(a_msg.message,a_msg.length);
        // DumpHex(message->transaction_msg.data,message->transaction_msg.len);
        // 		printf("date: %s\n",message->timestamp);
        // 		printf("from epoh:%lu\n",message->time_num);
    }
    // 	a_msg.message = msg_pointer;
    ipc_message__free_unpacked(message, NULL);
}

size_t send_ACKN_OK(struct io_uring *ring, int sock, void *buffer_wr)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    size_t n = send_ONLY_status_code(buffer_transactions, buffer_wr, IPC_MESSAGE__STATUS__ACKN_OK);
    io_uring_prep_send(sqe, sock, buffer_wr, n, MSG_DONTWAIT); // read answer
    if (client_logging_enabled)
        zlog_info(client_log, "sending acknowledgement!");
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    return n;
}

size_t send_STATUS(struct io_uring *ring, int sock, void *buffer_wr, IpcMessage__Status status_msg)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring); // add to ring
    size_t n = send_ONLY_status_code(buffer_transactions, buffer_wr, status_msg);
    if (client_logging_enabled)
        zlog_info(client_log, "sending status code %d!", status_msg);
    io_uring_prep_send(sqe, sock, buffer_wr, n, MSG_DONTWAIT); // read answer
    if (io_uring_submit(ring) < 0)
        printf("error submitting\n");
    return n;
}

/*

u_int64_t  get_epoch_ns(){
    long int ns;
    u_int64_t all;
    time_t sec;
    unsigned long one_bill = 1000000000L;
    struct timespec spec;

    if (clock_gettime(CLOCK_REALTIME,&spec)!=0){
        printf("ERROR GETTING EPOCH TIMESTAMP\n");
        all = 0;
        return all;
    }
    sec = spec.tv_sec;
    ns = spec.tv_nsec;
    all = (u_int64_t) sec *  one_bill + (u_int64_t) ns;
    return all;
}


long long get_date_usec_rec(){

    struct timeval ts;
    if (gettimeofday(&ts,NULL)) {
        printf("Error getting timestamp!\n");
        long long ret = 0;
        return ret;
    }
    long long ret = ts.tv_sec*1000LL + ts.tv_usec/1000;
    return ret;
}
*/

IpcMessage__Status read_response_ONLY_STATUS(void *buf, size_t len)
{
    IpcMessage__Status status;
    IpcMessage *message;
    message = ipc_message__unpack(0, len, buf);
    if (client_logging_enabled)
        zlog_info(client_log, "testing status code %d", message->status_code);
    status = message->status_code;
    ipc_message__free_unpacked(message, NULL);
    return status;
}

size_t get_a_message(void *buf, size_t len, signed_message_t *a_msg)
{
    IpcMessage *message;
    message = ipc_message__unpack(0, len, buf);
    if (message->has_pubkey)
    {
        memcpy(a_msg->public_key, message->pubkey.data, crypto_sign_PUBLICKEYBYTES);
    }
    if (message->has_transaction_msg)
    {
        a_msg->length = (unsigned long long)message->transaction_msg.len;
        memcpy(a_msg->message, message->transaction_msg.data, a_msg->length);
    }
    ipc_message__free_unpacked(message, NULL);
    return len;
}

/*
u_int64_t  get_epoch_ns_cl(){
    long int ns;
    u_int64_t all;
    time_t sec;
    unsigned long one_bill = 1000000000L;
    struct timespec spec;

    if (clock_gettime(CLOCK_REALTIME,&spec)!=0){
        printf("ERROR GETTING EPOCH TIMESTAMP\n");
        all = 0;
        return all;
    }
    sec = spec.tv_sec;
    ns = spec.tv_nsec;
    all = (u_int64_t) sec *  one_bill + (u_int64_t) ns;
    printf("EPOH %lu\n",all);
    return all;
}
*/
size_t send_ONLY_status_code(IpcMessage *message, void *socket_buf, IpcMessage__Status STATUS)
{
    int len = 0;
    char *date[32];
    memset(message, 0, sizeof(IpcMessage)); // clear
    ipc_message__init(message);

    message->has_pubkey = 0;
    message->has_transaction_msg = 0;

    size_t time_len = get_timestamp(date);
    message->timestamp = date; // its ok
    uint64_t epoch_ns = get_epoch_ns();
    message->time_num = epoch_ns;
    message->status_code = STATUS;

    len = ipc_message__get_packed_size(message);
    ipc_message__pack(message, socket_buf); // write to buffer
    return len;
}

/*
 * for debugging
 * */
/*
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
*/
