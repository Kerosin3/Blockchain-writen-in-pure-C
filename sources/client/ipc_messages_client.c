#include "ipc_messages_client.h"


IpcMessage__Status read_response_ONLY_STATUS(void* buf,size_t len){
	IpcMessage__Status status;	
	IpcMessage *message;
	message = ipc_message__unpack(0,len,buf);
	printf("STATUS CODE IS %d\n",message->status_code);
	printf("TIMESTAMP:%s\n",message->timestamp );
	status = message->status_code;
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
