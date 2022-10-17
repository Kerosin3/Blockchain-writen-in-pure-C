cmake_minimum_required(VERSION 3.14)
#project(serdes LANGUAGES C)

project(serdes)
ADD_LIBRARY( serdes STATIC
	serdes.c
	transaction.pb-c.c
	)

set(PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})

#ADD_EXECUTABLE (a.out
#		transaction.pb-c.c
#		main.c
#		)

TARGET_LINK_LIBRARIES(serdes
	              protobuf-c
	      		)
