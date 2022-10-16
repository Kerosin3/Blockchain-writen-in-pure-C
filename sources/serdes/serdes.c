#include "serdes.h"
/*
uint8_t puzzle_msg[10] = { 't','e','s','t','p','u','z','z','l','e' };

uint8_t test_pubkey[5] = { 0xAA, 0xAA,0xAA ,0xAA,0xEE};

void serialize_data(void* buffer);

size_t get_timestamp(void* buffer);



int main(int argc, char *argv[])
{
	void* buf = 0 ;
	serialize_data(buf);
	free(buf);
	

	char buffer[1024]= {0};
	int len = 0;
	FILE *fp;
	fp = fopen("serialized","rb");
	len = fread(buffer,sizeof(char),sizeof(buffer),fp);
	IpcMessage *message;
	message = ipc_message__unpack(0,len,buffer);
	if(message->has_pubkey){
		printf("has pubkey!\n");
		printf("%s\n",message->pubkey.data);
	}
	printf("has pubkey %d\n",message->has_pubkey);
	printf("has msg %d\n",message->has_transaction_msg);
	printf("date: %s\n",message->timestamp);
	ipc_message__free_unpacked(message,NULL);
	fclose(fp);
		

}

*/
size_t get_timestamp(void* buffer);


size_t serialize_data_v2(void* socket_buf,signed_message_t* a_message,IpcMessage* msg_buf){
	
	int len = 0;
	char* date[32];
	IpcMessage *message;
	message = (IpcMessage*) msg_buf;
	memset(message,0,sizeof(IpcMessage));
	ipc_message__init(message);
	message->has_transaction_msg = 1;

	message->transaction_msg.data = a_message->message;
	message->transaction_msg.len = a_message->length;

	message->has_pubkey = 1;

	message->pubkey.data = a_message->public_key;
	message->pubkey.len = crypto_sign_PUBLICKEYBYTES;

	message->status_code = IPC_MESSAGE__STATUS__MESSAGE_SENDED;
	
	message->time_num = get_epoch_ns();
	printf("SEREALIZED MESSAGE TIMESTAMP %lu",message->time_num);

	size_t time_len = get_timestamp(date);
	message->timestamp = date; // its ok

	len = ipc_message__get_packed_size(message);
	ipc_message__pack(message, socket_buf); // write to buffer
	printf("serialization done!\n");
	return len;

}


void print_serialized_data(void* buffer_in,size_t len){
	printf("\n");
// 	int len = 0;
//	FILE *fp;
//	fp = fopen("serialized","rb");
// 	len = fread(buffer,sizeof(char),sizeof(buffer),fp);
	IpcMessage *message;
	message = ipc_message__unpack(0,len,buffer_in);
	printf("status code:%d\n",message->status_code);
	if(message->has_pubkey){
		printf("has pubkey!\n");
		DumpHex(message->pubkey.data,message->pubkey.len);
	}
	if(message->has_transaction_msg){
		printf("decoded message:\n");
		DumpHex(message->transaction_msg.data, message->transaction_msg.len);
	}
// 	DumpHex(message->transaction_msg.data, message->transaction_msg.len);
//	printf("date: %s\n",message->timestamp);
	
	ipc_message__free_unpacked(message,NULL);
}

signed_message_t deserialize_data(int sock,void* deserialized_data){
	signed_message_t a_msg;
	int len = 0;
	len = read(sock,deserialized_data,4096);

	IpcMessage *message;
	message = ipc_message__unpack(0,len,deserialized_data);
	if ((message->status_code) ==IPC_MESSAGE__STATUS__OK )
		printf("OK!\n");
	if(message->has_pubkey){
		memcpy(a_msg.public_key,message->pubkey.data,crypto_sign_PUBLICKEYBYTES);
	}
	if(message->has_transaction_msg){
		a_msg.length = (unsigned long long) message->transaction_msg.len;
		memcpy(a_msg.message,message->transaction_msg.data,a_msg.length);
		printf("date: %s\n",message->timestamp);
	}
	ipc_message__free_unpacked(message,NULL);
		
	return a_msg;

}

/*
void serialize_data(void* buffer){
//	void* buffer;
	int len = 0;
	FILE *fp;
	char* date[32];
	IpcMessage *message;
	message = (IpcMessage*) malloc(sizeof(IpcMessage));
	ipc_message__init(message);
//	message->data = (Transaction*)malloc(sizeof(Transaction));
//	transaction__init(message->data);
	message->has_transaction_msg = 1;
	message->transaction_msg.data =    puzzle_msg;
	message->transaction_msg.len = 10;
	message->has_pubkey = 1;
	message->pubkey.data = test_pubkey;
	message->pubkey.len = 5;
	message->status_code = IPC_MESSAGE__STATUS__OK;
	get_timestamp(date);
	message->timestamp = date;

	len = ipc_message__get_packed_size(message);
	buffer = malloc(len);
	ipc_message__pack(message, buffer);

	fp = fopen("serialized","wb");
	
	fwrite(buffer,sizeof(void),len,fp);
		
	free(buffer);
//	free(message->data);
	free(message);
	fclose(fp);
	printf("serialized!\n");

}
*/
/*	
	char buffer[1024];
	int len = 0;
	FILE *fp;
	fp = fopen("serialized","rb");
	len = fread(buffer,sizeof(char),sizeof(buffer),fp);
	printf("len is %d\n",len);
	Somemessage *msg;
	msg = somemessage__unpack(NULL, len,buffer);
	printf("->%d\n",msg->len);
	printf("->%s\n",msg->name);
	printf("->%zu\n",msg->block.len);
	somemessage__free_unpacked(msg,NULL);
	fclose(fp);
	*/
/*
	Somemessage msg = SOMEMESSAGE__INIT;
	void *buf = 0; // storing serializing data
	unsigned len ; //
	msg.len =666;
	msg.name = "block1";
	msg.tnum = 1;
	msg.block.data  = puzzle_msg;
	msg.block.len = 10;
	len = somemessage__get_packed_size(&msg);
	buf = calloc(len,sizeof(char));
	somemessage__pack(&msg, buf );
	fprintf(stdout,"packet size is %d\n",len);
	FILE *fp;
	fp = fopen("serialized","ab");
	fwrite(buf,sizeof(void),len,fp);
	free(buf);
	fclose(fp);
*/


