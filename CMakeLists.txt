cmake_minimum_required(VERSION 3.1)
project("Berts Game")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wpedantic -march=native -mtune=native -g -I/opt/local/include")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L/opt/local/lib -L/usr/lib/i386-linux-gnu -lSDL2 -lSDL2_image -lSDL2_ttf")

add_executable(bgame main.cpp
	    engine/rng.cpp 
	    engine/virtual_terminal.cpp 
	    engine/command_manager.cpp 
	    engine/gui/gui.cpp
	    engine/gui/gui_frame.cpp 
	    engine/gui/gui_static_text.cpp 
	    engine/sdl2_backend.cpp 
	    engine/command_manager.cpp
	    engine/geometry.cpp
	    engine/png_writer.cpp
	    engine/perlin_noise.cpp
	    
	    game/game.cpp 
	    game/loading_screen.cpp 
	    game/main_menu.cpp
	    game/genesis_mode.cpp
	    game/game_mode.cpp
	    
	    game/render/panel_tooltip.cpp
	    game/render/window.cpp
	    game/render/settler_compositor.cpp
	    
	    game/world/gen/world_gen.cpp
	    game/world/planet.cpp
	    game/world/region.cpp
	    game/world/tile3d.cpp
	    game/world/gen/world_gen_biomes.cpp
	    game/world/gen/world_height_map_marching_squares.cpp
	    game/world/gen/world_gen_hydrology.cpp
	    game/world/gen/world_gen_layer_cake.cpp
	    game/world/gen/world_gen_noise.cpp
	    game/world/gen/tile_builder.cpp
	    
	    game/raws/raws.cpp 
	    game/raws/raw_power_generator.cpp	    
	    game/systems/system_factory.cpp 
	    game/systems/calendar_system.cpp 
	    game/systems/camera_system.cpp 
	    game/systems/obstruction_system.cpp 
	    game/systems/power_system.cpp 
	    game/systems/renderable_system.cpp 
	    game/systems/settler_ai_system.cpp 
	    game/systems/viewshed_system.cpp 
	    game/systems/input_system.cpp
	    game/systems/inventory_system.cpp
	    game/systems/cordex_ai_system.cpp
	    game/systems/damage_system.cpp
	    game/systems/particle_system.cpp
	    game/tasks/help_wanted.cpp
	    game/tasks/path_finding.cpp
	    game/tasks/find_nearest_provision.cpp
	    game/world/world.cpp 
	    game/world/console.cpp 
	    game/world/serialization_utils.cpp
	    game/world/universe.cpp
	    game/world/solar_system.cpp
	    game/world/solar_system_body.cpp
	    game/world/universegen.cpp
)
target_link_libraries(bgame)

# Packages:
# SDL2, SDL2Image, SDL2TTF

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(bgame ${SDL2_LIBRARY})

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

include_directories(${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(bgame ${SDL2IMAGE_LIBRARIES})

include_directories(${SDL2TTF_INCLUDE_DIRS})
target_link_libraries(bgame ${SDL2TTF_LIBRARIES})

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
find_package(Boost)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(bgame ${Boost_LIBRARIES})
endif()

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})
target_link_libraries(bgame ${PNG_LIBRARIES})

find_library(POCO_FOUNDATION PocoFoundation)
target_link_libraries(bgame ${POCO_FOUNDATION} ${POCO_NET})
