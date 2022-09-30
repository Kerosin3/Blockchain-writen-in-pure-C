#ADD_LIBRARY( servapplib STATIC
	#	     )

     #TARGET_LINK_LIBRARIES( servapplib 
	#picohttpparserlib
	#	)

ADD_EXECUTABLE (app.out
		main.c)

TARGET_LINK_LIBRARIES(app.out
		      tests
	              accounts
	      )
