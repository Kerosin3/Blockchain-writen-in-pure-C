project(networking)

ADD_LIBRARY( ${PROJECT_NAME} STATIC
	#	misc.c
	connection_handlers.c
	event_loop.c
	setup.c
	start_server.c
	ipcmessages.c
	data_handlers.c
	)



set(PROJECT_INCLUDE_DIRS ${SOURCES_ROOT_DIR} )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS}/${PROJECT_NAME}) #import headers

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIRS}/) #export headers

#setup doxygen
find_package(Doxygen 
	     REQUIRED dot 
	     OPTIONAL_COMPONENTS mscgen dia)
if (DOXYGEN_FOUND)
	set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxy_setup/Doxyfile)
	set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
	message("starting doxygen guild")

	add_custom_target(doc_doxygen_${PROJECT_NAME} ALL
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "generating doc for networking"
		VERBATIM
	)
else (DOXYGEN_FOUND)
	message("doxygen not found")
endif(DOXYGEN_FOUND)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
		      uring
		      protobuf
		      serdes
		      misc
		      blockdata
		      logging
		      )
