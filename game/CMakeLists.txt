add_library(game loading_screen.cpp main_menu.cpp play_mode.cpp world_gen_mode.cpp gui/gui_console.cpp gui/gui_main_game_command_panel.cpp gui/gui_main_game_view.cpp
	    raws/raws.cpp raws/raw_power_generator.cpp
	    systems/system_factory.cpp systems/calendar_system.cpp systems/camera_system.cpp systems/obstruction_system.cpp 
	    systems/power_system.cpp systems/renderable_system.cpp systems/settler_ai_system.cpp systems/viewshed_system.cpp
	    systems/flowmap_system.cpp systems/input_system.cpp
	    world/geometry.cpp world/world.cpp world/tile.cpp world/landblock.cpp world/console.cpp world/worldgen.cpp world/serialization_utils.cpp
)
