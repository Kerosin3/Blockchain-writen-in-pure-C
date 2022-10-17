project(networking)

ADD_LIBRARY( networking STATIC
	misc.c
	connection_handlers.c
	event_loop.c
	setup.c
	start_server.c
	setup_client.c
	ipcmessages.c
	data_handlers.c
	)

set(PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(networking
		      uring
		      protobuf
		      )
