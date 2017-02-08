add_library(components STATIC
	calendar.cpp
	game_stats.cpp
	logger.cpp
	species.cpp
)
target_link_libraries(components rltk)
target_link_libraries(components planet)
cotire(components)