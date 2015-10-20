add_library(engine engine.cpp rng.cpp virtual_terminal.cpp ncurses_backend.cpp sdl2_backend.cpp ansi_backend.cpp command_manager.cpp)
add_subdirectory(gui)
add_subdirectory(ecs)
