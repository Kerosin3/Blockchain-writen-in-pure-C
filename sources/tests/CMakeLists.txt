project(tests)
ADD_LIBRARY( ${PROJECT_NAME} STATIC
	tests.c
	tests2.c
	)
#set(PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ )

#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})


set(PROJECT_INCLUDE_DIRS ${SOURCES_ROOT_DIR} )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS}/${PROJECT_NAME}) #import headers

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIRS}/) #export headers


#message( STATUS "prj includ edirs " ${PROJECT_INCLUDE_DIRS} )

TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
		      misc
		      accounts
		      hashing
		      blockdata
		      sodium
		      puzzle
		      networking
		      protobuf-c
		      serdes
		      logging
		      )
