project(logging)



#ADD_LIBRARY( inetmisc STATIC
#	inet_misc.c
#	)


set(PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})



#TARGET_LINK_LIBRARIES(inetmisc
#			sodium
#			accounts
#		      )
