project(accounts)
ADD_LIBRARY(accounts OBJECT
	acc_utils.c
	)

set(PROJECT_INCLUDE_DIRS ${SOURCES_ROOT_DIR} )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS}/${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIRS}/) #export headers
TARGET_LINK_LIBRARIES(accounts
		      sodium
		      )
