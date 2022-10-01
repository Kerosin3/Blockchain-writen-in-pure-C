ADD_LIBRARY( hashing STATIC
	hashing.c
	)

TARGET_LINK_LIBRARIES(hashing
		      sodium
		      )
