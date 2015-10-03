add_library(engine engine.cpp rng.cpp virtual_terminal.cpp ncurses_backend.cpp sdl2_backend.cpp)
add_subdirectory(gui)
