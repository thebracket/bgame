cmake_minimum_required(VERSION 3.1)

# Build Lua
add_subdirectory(lua)
include_directories(${lua_BINARY_DIR})

# Build SFML
add_subdirectory(sfml)

# Build Zlib
add_subdirectory(zlib)
include_directories(${zlib_BINARY_DIR})
set_target_properties(example PROPERTIES EXCLUDE_FROM_ALL 1)
set_target_properties(minigzip PROPERTIES EXCLUDE_FROM_ALL 1)

# Build RLTK
add_library(rltk STATIC rltk/rltk/rltk.cpp
	rltk/rltk/texture_resources.cpp
	rltk/rltk/color_t.cpp
	rltk/rltk/virtual_terminal.cpp
	rltk/rltk/rng.cpp
	rltk/rltk/geometry.cpp
	rltk/rltk/input_handler.cpp
	rltk/rltk/font_manager.cpp
	rltk/rltk/gui.cpp
	rltk/rltk/layer_t.cpp
	rltk/rltk/gui_control_t.cpp
	rltk/rltk/virtual_terminal_sparse.cpp
	rltk/rltk/ecs.cpp
	rltk/rltk/xml.cpp
	rltk/rltk/perlin_noise.cpp
	rltk/rltk/rexspeeder.cpp
	rltk/rltk/scaling.cpp
	)
target_link_libraries(rltk sfml-system sfml-graphics sfml-audio sfml-network sfml-window)
target_link_libraries(rltk zlibstatic)

# Build sqlite
add_library(sqlite STATIC sqlite/sqlite3.c)

# Build FastNoise
add_library(FastNoise STATIC FastNoise/FastNoise.cpp)

# Build imgui
add_library(imgui STATIC imgui/imgui.cpp imgui/imgui_draw.cpp)

# Build imgi-sfml
include_directories(imgui)
include_directories(imgui-sfml)
add_library(imgui-sfml STATIC imgui-sfml/imgui-SFML.cpp )
target_link_libraries(imgui-sfml imgui)
