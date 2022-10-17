
project(blockdata)

ADD_LIBRARY( blockdata STATIC
	blockdata.c
	layering.c
	)

set(PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(hashing
		      sodium
		      misc
		      )
