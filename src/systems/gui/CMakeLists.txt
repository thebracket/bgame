add_library(gui 
	hud_system.cpp
	log_system.cpp
)
target_link_libraries(gui components)
#cotire(gui)
