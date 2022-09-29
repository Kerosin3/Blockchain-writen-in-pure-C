ADD_LIBRARY( accounts STATIC
		accounts.c

TARGET_LINK_LIBRARIES(accounts
	              openssl
		      )
