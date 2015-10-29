add_library(game loading_screen.cpp main_menu.cpp play_mode.cpp world_gen_mode.cpp gui/gui_console.cpp gui/gui_main_game_command_panel.cpp gui/gui_main_game_view.cpp)
add_subdirectory(world)
add_subdirectory(systems)