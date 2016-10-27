cmake_minimum_required(VERSION 3.1)

# Boost
include(${PROJECT_SOURCE_DIR}/../cmake_modules/utils.cmake)
include(${PROJECT_SOURCE_DIR}/../cmake_modules/add_boost.cmake)

# Build Lua
add_subdirectory(lua)

# Build SFML
add_subdirectory(sfml)

# Build Zlib
add_subdirectory(zlib)
set_target_properties(example PROPERTIES EXCLUDE_FROM_ALL 1)
set_target_properties(minigzip PROPERTIES EXCLUDE_FROM_ALL 1)

# Build RLTK
include_directories(${BoostSourceDir})
add_library(rltk rltk/rltk/rltk.cpp
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
	rltk/rltk/perlin_noise.cpp
	rltk/rltk/rexspeeder.cpp
	)
target_link_libraries(rltk ${BoostSystemLibs} ${BoostFilesystemLibs} ${BoostContainerLibs})
target_link_libraries(rltk sfml-system sfml-graphics sfml-audio sfml-network sfml-window)
target_link_libraries(rltk zlibstatic)

# Build FastNoise
add_library(FastNoise FastNoise/FastNoise.cpp)
