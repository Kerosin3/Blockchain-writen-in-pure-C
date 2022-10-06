ADD_LIBRARY( blockdata STATIC
	blockdata.c
	layering.c
	)

TARGET_LINK_LIBRARIES(hashing
		      sodium
		      )
