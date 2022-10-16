cmake_minimum_required(VERSION 3.14)
project(client LANGUAGES C)


ADD_EXECUTABLE (client.out
		setup_client.c
		ipc_messages_client.c
		../serdes/transaction.pb-c.c
		block_process.c
		main.c
		)

TARGET_LINK_LIBRARIES(client.out
		      uring
	              protobuf-c
		      blockdata
		      hashing
		      accounts
		      misc
	      		)
