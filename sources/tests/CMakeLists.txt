ADD_LIBRARY( tests STATIC
	tests.c
	)

TARGET_LINK_LIBRARIES(tests
		      misc
		      accounts
		      hashing
		      sodium
		      )
