ADD_LIBRARY( accounts STATIC
	activation.c
	acc_utils.c
	)

TARGET_LINK_LIBRARIES(accounts
		      sodium
		      )
