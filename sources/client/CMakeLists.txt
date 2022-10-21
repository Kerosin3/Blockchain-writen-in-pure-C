cmake_minimum_required(VERSION 3.14)
project(client LANGUAGES C)


ADD_EXECUTABLE ( ${PROJECT_NAME}
		setup_client.c
		ipc_messages_client.c
		../serdes/transaction.pb-c.c
		block_process.c
		main.c
		)

	#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})

set(PROJECT_INCLUDE_DIRS ${SOURCES_ROOT_DIR} )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES( ${PROJECT_NAME}
		      uring
	              protobuf-c
		      misc
		      blockdata
		      hashing
		      accounts
	      		)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "client.app")
