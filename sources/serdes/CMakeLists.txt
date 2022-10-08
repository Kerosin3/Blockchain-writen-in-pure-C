cmake_minimum_required(VERSION 3.14)
#project(serdes LANGUAGES C)

ADD_LIBRARY( serdes STATIC
	serdes.c
	transaction.pb-c.c
	)


#ADD_EXECUTABLE (a.out
#		transaction.pb-c.c
#		main.c
#		)

TARGET_LINK_LIBRARIES(serdes
	              protobuf-c
	      		)
