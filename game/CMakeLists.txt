add_library(game 
	    game.cpp 
	    loading_screen.cpp 
	    main_menu.cpp 
	    play_mode.cpp 
	    world_gen_mode.cpp
	    
	    render/game_render.cpp
	    render/panel_tooltip.cpp
	    render/window.cpp
	    render/settler_compositor.cpp
	    
	    raws/raws.cpp 
	    raws/raw_power_generator.cpp	    
	    systems/system_factory.cpp 
	    systems/calendar_system.cpp 
	    systems/camera_system.cpp 
	    systems/obstruction_system.cpp 
	    systems/power_system.cpp 
	    systems/renderable_system.cpp 
	    systems/settler_ai_system.cpp 
	    systems/viewshed_system.cpp 
	    systems/input_system.cpp
	    systems/inventory_system.cpp
	    systems/cordex_ai_system.cpp
	    systems/damage_system.cpp
	    tasks/help_wanted.cpp
	    tasks/path_finding.cpp
	    tasks/find_nearest_provision.cpp
	    world/geometry.cpp 
	    world/world.cpp 
	    world/tile.cpp 
	    world/landblock.cpp 
	    world/console.cpp 
	    world/worldgen.cpp 
	    world/serialization_utils.cpp)
	    