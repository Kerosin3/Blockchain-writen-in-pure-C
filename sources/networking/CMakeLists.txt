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

TARGET_LINK_LIBRARIES(networking
		      uring
		      protobuf
		      )
