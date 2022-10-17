project(misc)
ADD_LIBRARY( misc OBJECT
	misc.c
	)
set(PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
#add_library(${PROJECT_NAME} OBJECT ${PROJECT_SOURCES})

#TARGET_LINK_LIBRARIES(accounts
#		      sodium
#		      )
