ADD_LIBRARY( inetmisc STATIC
	inet_misc.c
	)

TARGET_LINK_LIBRARIES(inetmisc
			sodium
			accounts
		      )
