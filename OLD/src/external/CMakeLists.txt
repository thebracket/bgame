cmake_minimum_required(VERSION 3.1)

# We depend upon zlib
find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
        include_directories(${ZLIB_INCLUDE_DIRS})
endif()

# Build RLTK
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
	rltk/rltk/xml.cpp
	rltk/rltk/perlin_noise.cpp
	rltk/rltk/rexspeeder.cpp
	rltk/rltk/scaling.cpp
	)
target_link_libraries(rltk ${ZLIB_LIBRARIES})
target_link_libraries(rltk ${SFML_LIBRARIES})
cotire(rltk)

# Build sqlite
add_library(sqlite STATIC sqlite/sqlite3.c)
cotire(sqlite)

# Build FastNoise
add_library(FastNoise STATIC FastNoise/FastNoise.cpp)
cotire(FastNoise)

# Build imgui
add_library(imgui STATIC imgui/imgui.cpp imgui/imgui_draw.cpp)
target_link_libraries(imgui ${SFML_LIBRARIES})
cotire(imgui)

# Build imgi-sfml
include_directories(imgui)
include_directories(imgui-sfml)
add_library(imgui-sfml imgui-sfml/imgui-SFML.cpp )
target_link_libraries(imgui-sfml ${SFML_LIBRARIES})
target_link_libraries(imgui-sfml imgui)
target_link_libraries(imgui-sfml ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
cotire(imgui-sfml)
