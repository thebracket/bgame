add_library(bfmain STATIC
    menu_helper.cpp
	splash_screen.cpp
	main_menu.cpp
	world_gen.cpp
	play_game.cpp
	game_config.cpp
	game_planet.cpp
	game_clipping.cpp
	game_pause.cpp
	game_mode.cpp
	game_calendar.cpp
	game_designations.cpp
	game_camera.cpp
	game_building.cpp
	game_logger.cpp
	game_selections.cpp
	game_rng.cpp
	game_region.cpp
)
target_link_libraries(bfmain planet rltk components systems utils gl)
cotire(bfmain)
