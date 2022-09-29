ADD_LIBRARY( accounts STATIC
	activation.c
	)

TARGET_LINK_LIBRARIES(accounts
		      sodium
		      )
