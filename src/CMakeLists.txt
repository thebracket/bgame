cmake_minimum_required(VERSION 3.1)
project("Black Future")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake_modules)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wpedantic -march=native -mtune=native -pthread -g")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L/opt/local/lib -L/opt/local/lib/lua-5.2 -pthread -flto")

include_directories(rltk/rltk)

# Require Boost
set(Boost_USE_STATIC_LIBS      OFF) # only find static libs
set(Boost_USE_MULTITHREADED    ON)
set(Boost_USE_STATIC_RUNTIME   OFF)
find_package(Boost COMPONENTS filesystem system iostreams REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

# Require LUA - experimental
set(LUA_LIBRARIES	lua-5.2)
find_package(Lua REQUIRED)
if(LUA_FOUND)
	include_directories(${LUA_INCLUDE_DIR})
endif()


add_library(FastNoise FastNoise/FastNoise.cpp)

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

add_subdirectory(components)
add_subdirectory(planet)
add_subdirectory(raws)
add_subdirectory(systems)
add_subdirectory(utils)
add_subdirectory(main)

add_executable(bgame main.cpp)
add_executable(png2rex png2rex/main.cpp png2rex/PngLoader.cpp png2rex/REXSpeeder.cpp)

if (APPLE)
	set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
	set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2015-Present Bracket Productions")
	set(MACOSX_BUNDLE_BUNDLE_NAME "Black Future")
	set(MACOSX_BUNDLE_ICON_FILE "icon")
	set(MACOSX_BUNDLE_BUNDLE_VERSION "0.10.0")
	set(myApp_ICON ${CMAKE_CURRENT_SOURCE_DIR}/../assets/icon.icns)
	set_source_files_properties(${myApp_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
	add_executable(bf MACOSX_BUNDLE ${myApp_ICON} main.cpp)
	target_link_libraries(bf)
	target_link_libraries(bf rltk)
	target_link_libraries(bf FastNoise)
	target_link_libraries(bf components)
	target_link_libraries(bf planet)
	target_link_libraries(bf raws)
	target_link_libraries(bf systems)
	target_link_libraries(bf utils)
	target_link_libraries(bf bfmain)
	target_link_libraries(bf ${LUA_LIBRARIES})
#	fixup_bundle(${CMAKE_INSTALL_PREFIX}/bf.app "" "")
	install(CODE "
	include (BundleUtilities)
	fixup_bundle(${CMAKE_INSTALL_PREFIX}/bf.app \"\" \"\")
	" COMPONENT bf)
	add_custom_command(
		TARGET bf
		COMMAND mkdir -p bf.app/Contents/MacOS/assets
		COMMAND mkdir -p bf.app/Contents/MacOS/world
		COMMAND mkdir -p bf.app/Contents/MacOS/world_defs
		COMMAND mkdir -p bf.app/Contents/MacOS/rex
		COMMAND rm -f bf.app/Contents/MacOS/world/*
		COMMAND cp ../assets/fonts.json bf.app/Contents/MacOS/assets/
		COMMAND cp ../assets/terminal8x8.png bf.app/Contents/MacOS/assets/
		COMMAND cp ../assets/terminal16x16.png bf.app/Contents/MacOS/assets/
		COMMAND cp ../assets/terminal32x32.png bf.app/Contents/MacOS/assets/
		COMMAND cp ../assets/VGA8x16.png bf.app/Contents/MacOS/assets/
		COMMAND cp ../assets/background_image.png bf.app/Contents/MacOS/assets/
		COMMAND cp ../assets/gamelogo.png bf.app/Contents/MacOS/assets/
		COMMAND cp -R ../world_defs/* bf.app/Contents/MacOS/world_defs
		COMMAND cp -R ../rex/* bf.app/Contents/MacOS/rex
	)
endif()

target_link_libraries(rltk ${SFML_LIBRARIES})
target_link_libraries(bgame)
target_link_libraries(bgame rltk)
target_link_libraries(bgame FastNoise)
target_link_libraries(bgame components)
target_link_libraries(bgame planet)
target_link_libraries(bgame raws)
target_link_libraries(bgame bfmain)
target_link_libraries(rltk ${Boost_LIBRARIES})
target_link_libraries(bgame ${LUA_LIBRARIES})
target_link_libraries(bgame utils)
target_link_libraries(bgame systems)

# We depend upon zlib
find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
	target_link_libraries(rltk ${ZLIB_LIBRARIES})
	target_link_libraries(png2rex ${ZLIB_LIBRARIES})
endif()

# We also depend upon libpng for the png tool
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})
target_link_libraries(png2rex ${PNG_LIBRARIES})

