project(misc)

ADD_LIBRARY( misc OBJECT
	misc.c
	)
#include from INCLUDE
set(PROJECT_INCLUDE_DIRS ${SOURCES_ROOT_DIR} )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS}/${PROJECT_NAME})

#message( STATUS "*****---misc" ${PROJECT_INCLUDE_DIRS} )
