project(tests)
ADD_LIBRARY( tests STATIC
	tests.c
	tests2.c
	)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
#set(PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ )

message( STATUS "*****---" ${PROJECT_INCLUDE_DIRS} )
TARGET_LINK_LIBRARIES(tests
		      misc
		      accounts
		      hashing
		      blockdata
		      sodium
		      puzzle
		      networking
		      protobuf-c
		      serdes
		      )
