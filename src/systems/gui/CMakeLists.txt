add_library(gui 
	hud_system.cpp
	log_system.cpp
	tooltip_system.cpp
	civ_ui_system.cpp
)
target_link_libraries(gui components globalassets)
#cotire(gui)
