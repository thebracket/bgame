cmake_minimum_required(VERSION 3.1)
project("Berts Game")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -march=native -mtune=native -funroll-loops -ffast-math -Wall -Wpedantic -g -I/opt/local/include")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L/opt/local/lib -L/usr/lib/i386-linux-gnu -lSDL2 -lSDL2_image -lncursesw")

add_subdirectory(engine)
add_executable(bgame main.cpp
	    
	    game/game.cpp 
	    game/loading_screen.cpp 
	    game/main_menu.cpp 
	    game/play_mode.cpp 
	    game/world_gen_mode.cpp 
	    game/gui/gui_main_game_view.cpp
	    
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
	    game/systems/flowmap_system.cpp 
	    game/systems/input_system.cpp
	    game/world/geometry.cpp 
	    game/world/world.cpp 
	    game/world/tile.cpp 
	    game/world/landblock.cpp 
	    game/world/console.cpp 
	    game/world/worldgen.cpp 
	    game/world/serialization_utils.cpp
)
target_link_libraries(bgame bengine)

# Packages:
# SDL2, SDL2Image

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(bgame ${SDL2_LIBRARY})

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(bgame ${SDL2IMAGE_LIBRARIES})
