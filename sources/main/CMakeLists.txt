project(main)

ADD_EXECUTABLE (app.out
		main.c)

set(PROJECT_INCLUDE_DIRS ${SOURCES_ROOT_DIR} )

TARGET_LINK_LIBRARIES(app.out
		      tests
	              accounts
	      )
