add_library(globalassets
        game_building.cpp
		game_calendar.cpp
		game_camera.cpp
		game_config.cpp
		game_designations.cpp
		game_logger.cpp
		game_mode.cpp
		game_pause.cpp
		game_planet.cpp
		rng.cpp
		shader_storage.cpp
		spatial_db.cpp
		texture_storage.cpp
        )
target_link_libraries(globalassets shaders)
cotire(globalassets)
