cmake_minimum_required(VERSION 2.8)
project("Berts Game")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Ofast -g -I/opt/local/include")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L/opt/local/lib -lSDL2 -lSDL2_image -lncursesw")

add_subdirectory(engine)
add_subdirectory(game)
add_executable(bgame game main.cpp)
target_link_libraries(bgame engine game gui world)

#set(CURSES_NEED_WIDE TRUE)
#find_package(Curses REQUIRED)
#include_directories(${CURSES_INCLUDE_DIRS})
#target_link_libraries (bgame ${CURSES_LIBRARIES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(bgame ${SDL2_LIBRARY})
