#ADD_LIBRARY( servapplib STATIC
	#	     )

     #TARGET_LINK_LIBRARIES( servapplib 
	#picohttpparserlib
	#	)

ADD_EXECUTABLE (a.out
		main.c)

TARGET_LINK_LIBRARIES(app.out
	protobuf-c
	      )
