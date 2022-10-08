ADD_LIBRARY( tests STATIC
	tests.c
	tests2.c
	)

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
