add_library(bfmain STATIC
    menu_helper.cpp
	splash_screen.cpp
	main_menu.cpp
	world_gen.cpp
	play_game.cpp
	game_globals.cpp
)
target_link_libraries(bfmain planet rltk components systems)

