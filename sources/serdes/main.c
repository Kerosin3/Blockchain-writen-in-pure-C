#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "transaction.pb-c.h"

uint8_t puzzle_msg[10] = { 't','e','s','t','p','u','z','z','l','e' };

uint8_t test_pubkey[5] = { 0x20, 0x40,0xEA ,0xAA,0xEE};

int main(int argc, char *argv[])
{

	void* buffer;
	int len = 0;
	FILE *fp;

	IpcMessage *message;
	message = (IpcMessage*) malloc(sizeof(IpcMessage));
	ipc_message__init(message);
	message->data = (Transaction*)malloc(sizeof(Transaction));
	transaction__init(message->data);
	
	message->status_code = IPC_MESSAGE__STATUS__OK;
	message->data->date = "11.10.2022";
	message->data->transaction.data = puzzle_msg;
	message->data->transaction.len = 10;
	message->data->pubkey.data = test_pubkey;
	message->data->pubkey.len = 5;
	message->status_code = IPC_MESSAGE__STATUS__OK;
	printf("haha\n");

	len = ipc_message__get_packed_size(message);
	buffer = malloc(len);
	ipc_message__pack(message, buffer);

	fp = fopen("serialized","wb");
	
	fwrite(buffer,sizeof(void),len,fp);
		
	free(buffer);
	free(message->data);
	free(message);
	fclose(fp);
	
}
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


