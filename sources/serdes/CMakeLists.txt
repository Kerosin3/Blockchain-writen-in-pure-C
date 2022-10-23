cmake_minimum_required(VERSION 3.14)
#project(serdes LANGUAGES C)

project(serdes)
ADD_LIBRARY( ${PROJECT_NAME} STATIC
	serdes.c
	transaction.pb-c.c
	)


set(PROJECT_INCLUDE_DIRS ${SOURCES_ROOT_DIR} )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS}/${PROJECT_NAME}) #import headers

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIRS}/) #export headers


#ADD_EXECUTABLE (a.out
#		transaction.pb-c.c
#		main.c
#		)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	              protobuf-c
		      misc
		      accounts
	      		)
