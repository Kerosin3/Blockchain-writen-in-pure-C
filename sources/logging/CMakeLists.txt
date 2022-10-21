project(logging)


SET(ZLOG_SOURCE_DIR ${CMAKE_SOURCE_DIR}/sources/contrib/zlog/src/ )

set(PROJECT_INCLUDE_DIRS ${SOURCES_ROOT_DIR} )

file(GLOB zlog_src CMAKE_CONFIGURE_DEPENDS
	${ZLOG_SOURCE_DIR}*.c
	${ZLOG_SOURCE_DIR}*.h
	)
ADD_LIBRARY(${PROJECT_NAME} STATIC  ${zlog_src})


#message(STATUS "----->" ${CMAKE_SOURCE_DIR}/sources/contrib/zlog/src/ )

set(PROJECT_INCLUDE_DIRS ${SOURCES_ROOT_DIR} )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS}/${PROJECT_NAME})



