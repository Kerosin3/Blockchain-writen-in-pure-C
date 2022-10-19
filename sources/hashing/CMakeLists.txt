project(hashing)


ADD_LIBRARY( ${PROJECT_NAME} OBJECT
	hashing.c
	)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
		      sodium
		      misc
		      )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
message( STATUS "*****" ${PROJECT_INCLUDE_DIRS} )

