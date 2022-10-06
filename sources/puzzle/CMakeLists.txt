ADD_LIBRARY( puzzle STATIC
	puzzle.c
	)

TARGET_LINK_LIBRARIES(puzzle
		      sodium
		      misc
		      )
