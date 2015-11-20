cmake_minimum_required(VERSION 3.1)
project("Berts Game")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native -mtune=native -funroll-loops -ffast-math -Wall -Wpedantic -g -I/opt/local/include")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L/opt/local/lib -L/usr/lib/i386-linux-gnu -lSDL2 -lSDL2_image -lSDL2_ttf")

add_subdirectory(engine)
add_subdirectory(game)
add_executable(bgame main.cpp)
target_link_libraries(bgame game bengine)

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
