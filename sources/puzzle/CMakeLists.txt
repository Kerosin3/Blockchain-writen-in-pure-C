project(puzzle)


ADD_LIBRARY( puzzle OBJECT
	puzzle.c
	)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(puzzle
		      sodium
		      misc
		      )
