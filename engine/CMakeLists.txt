add_library(engine rng.cpp virtual_terminal.cpp backends/ncurses_backend.cpp backends/sdl2_backend.cpp 
	    backends/ansi_backend.cpp backends/command_manager.cpp globals.cpp gui/gui.cpp
	    gui/gui_frame.cpp gui/gui_static_text.cpp)
add_subdirectory(ecs)
add_subdirectory(raws)

add_subdirectory(backends)