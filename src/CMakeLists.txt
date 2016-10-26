cmake_minimum_required(VERSION 3.1)
project("Black Future")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake_modules)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wpedantic -march=native -mtune=native -pthread -g -fopenmp")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L/opt/local/lib -pthread -flto")

include_directories(external/sfml/include)
include_directories(external/rltk/rltk)
include_directories(external/FastNoise)
include_directories(external/lua/src)
include_directories(external)

# Require Boost
set(Boost_USE_STATIC_LIBS      OFF) # only find static libs
set(Boost_USE_MULTITHREADED    ON)
set(Boost_USE_STATIC_RUNTIME   OFF)
find_package(Boost COMPONENTS filesystem system iostreams REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

add_subdirectory(external)

add_subdirectory(components)
add_subdirectory(planet)
add_subdirectory(raws)
add_subdirectory(systems)
add_subdirectory(utils)
add_subdirectory(main)

add_executable(bgame main.cpp)

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

target_link_libraries(bgame)
target_link_libraries(bgame rltk)
target_link_libraries(bgame FastNoise)
target_link_libraries(bgame components)
target_link_libraries(bgame planet)
target_link_libraries(bgame raws)
target_link_libraries(bgame bfmain)
target_link_libraries(bgame utils)
target_link_libraries(bgame systems)
