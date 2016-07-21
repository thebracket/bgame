cmake_minimum_required(VERSION 3.1)
project("Berts Game")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake_modules)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -O3 -Wall -Wpedantic -march=native -mtune=native -g")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L/opt/local/lib -L/opt/local/lib/lua-5.2")

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

# Require SFML
find_package(SFML 2 COMPONENTS system window graphics REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

add_executable(bgame main.cpp
	menu_helper.cpp
	splash_screen.cpp
	main_menu.cpp
	world_gen.cpp
	play_game.cpp
	game_globals.cpp
	components/loader.cpp
	components/building.cpp
	components/calendar.cpp
	components/game_stats.cpp
	systems/map_render_system.cpp
	systems/calendar_system.cpp
	systems/settler_ai_system.cpp
	systems/panel_render_system.cpp
	systems/mining_system.cpp
	systems/inventory_system.cpp
	systems/path_finding.cpp
	systems/visibility_system.cpp
	systems/workflow_system.cpp
	systems/topology_system.cpp
	planet/planet.cpp
	planet/region.cpp
	planet/planet_builder.cpp
	planet/builder/settler_builder.cpp
	planet/builder/map_builder.cpp
	planet/builder/biome_builder.cpp
	planet/builder/region_builder.cpp
	raws/raws.cpp
	raws/lua_bridge.cpp
)
target_include_directories(bgame PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/rltk/rltk>
)

target_link_libraries(bgame)
target_link_libraries(rltk ${SFML_LIBRARIES})
target_link_libraries(bgame rltk)

# Require Boost
set(Boost_USE_STATIC_LIBS      OFF) # only find static libs
set(Boost_USE_MULTITHREADED    ON)
set(Boost_USE_STATIC_RUNTIME   OFF)
find_package(Boost COMPONENTS filesystem system iostreams REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(rltk ${Boost_LIBRARIES})
endif()

# Require LUA - experimental
set(LUA_LIBRARIES	lua-5.2)
find_package(Lua REQUIRED)
if(LUA_FOUND)
	include_directories(${LUA_INCLUDE_DIR})
	target_link_libraries(bgame ${LUA_LIBRARIES})
endif()

# We depend upon zlib
find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
	target_link_libraries(rltk ${ZLIB_LIBRARIES})
endif()
