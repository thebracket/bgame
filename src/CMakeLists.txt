cmake_minimum_required(VERSION 3.1)
project("Black Future")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake_modules)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
if (NOT(WIN32))
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wpedantic -march=native -mtune=native -pthread -g -fopenmp -ffast-math")
	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L/opt/local/lib -pthread -flto")
endif()
if (WIN32)
	set(CMAKE_BUILD_TYPE RELEASE)
	set(CMAKE_CXX_FLAGS "/Ox /GL /openmp /EHr /EHsc")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /TC /Ox /GL /openmp")
	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
endif()

option(USE_BOOST_CACHE "If ON, try to download, extract and build boost to a single cache directory for all MaidSafe clones" ON)
option(BOOST_DISABLE_ASSERTS "If ON, all BOOST_ASSERTs and BOOST_VERIFYs are disabled" OFF)

include(cotire)

# Windows shims
if (WIN32)
	include_directories(external/fixwindows)
else()
	include_directories(external/fixothers)
endif()
include_directories(external/sfml/include)
include_directories(external/rltk/rltk)
include_directories(external/FastNoise)
include_directories(external/lua/src)
include_directories(external/zlib)
include_directories(external)

add_subdirectory(external)
include_directories(${BoostSourceDir})

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
    cotire(bf)
	target_link_libraries(bf)
	target_link_libraries(bf rltk)
	target_link_libraries(bf FastNoise)
	target_link_libraries(bf components)
	target_link_libraries(bf planet)
	target_link_libraries(bf raws)
	target_link_libraries(bf systems)
	target_link_libraries(bf utils)
	target_link_libraries(bf bfmain)
	target_link_libraries(bf sqlite)
    target_link_libraries(bf imgui)
    target_link_libraries(bf imgui-sfml)
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
        COMMAND cp ../assets/Nagidal24x24shade.png bf.app/Contents/MacOS/assets/
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
target_link_libraries(bgame sqlite)
target_link_libraries(bgame imgui)
target_link_libraries(bgame imgui-sfml)

cotire(bgame)
