#include "ipc_messages_client.h"


size_t get_timestamp(void* buffer){
	char date[32];
        time_t t = time(NULL);
    	struct tm* tm = gmtime(&t);
    	strftime(date, sizeof(date), "%d/%m/%Y %H:%M:%S GMT", tm);
	int n = snprintf(buffer, strlen(date)+1, "%s", date);
	return n;
}


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


IpcMessage__Status read_response_ONLY_STATUS(void* buf,size_t len){
	IpcMessage__Status status;	
	IpcMessage *message;
	message = ipc_message__unpack(0,len,buf);
	printf(" TESTING STATUS CODE IS %d\n",message->status_code);
//	printf("TIMESTAMP:%s\n",message->timestamp );
	status = message->status_code;
        printf("TIMESTAMP:%ld\n",message->time_num);
	ipc_message__free_unpacked(message,NULL);
	return status;
}

size_t get_a_message(void* buf,size_t len,signed_message_t* a_msg){
	IpcMessage* message;
	message = ipc_message__unpack(0,len, buf );
	if ((message->status_code) ==IPC_MESSAGE__STATUS__OK )
		printf("OK!\n");
	if(message->has_pubkey){
		memcpy(a_msg->public_key,message->pubkey.data,crypto_sign_PUBLICKEYBYTES);
	}
	if(message->has_transaction_msg){
		a_msg->length = (unsigned long long) message->transaction_msg.len;
		memcpy(a_msg->message,message->transaction_msg.data,a_msg->length);
	}
	ipc_message__free_unpacked(message,NULL);
	return len;	

}


size_t send_ONLY_status_code( IpcMessage* message,void* socket_buf, IpcMessage__Status STATUS){
	int len = 0;
	char* date[32];
	memset(message,0,sizeof(IpcMessage)); //clear
	ipc_message__init(message);

	message->has_pubkey = 0;
	message->has_transaction_msg = 0;
	
	size_t time_len = get_timestamp(date);
	message->timestamp = date; // its ok
	message->time_num = get_epoch_ns();
	message->status_code = STATUS;
        printf("TIMESTAMP SENDED MESSAGE:%ld\n",message->time_num);
	
	len = ipc_message__get_packed_size(message);
	ipc_message__pack(message, socket_buf); // write to buffer
	return len;

}

