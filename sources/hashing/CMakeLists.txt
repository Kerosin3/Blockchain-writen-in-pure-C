project(hashing)


ADD_LIBRARY( hashing OBJECT
	hashing.c
	)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(hashing
		      sodium
		      misc
		      )
