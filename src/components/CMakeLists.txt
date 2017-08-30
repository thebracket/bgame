add_library(components
	calendar.cpp
	game_stats.cpp
	item.cpp
	logger.cpp
	species.cpp
)
target_link_libraries(components planet)
#cotire(components)
