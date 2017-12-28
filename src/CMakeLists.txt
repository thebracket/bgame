cmake_minimum_required(VERSION 3.7)
project("Nox Futura")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake_modules)
include(cotire)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
	message("Using ccache")
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR -Wno-deprecated-declarations -O3 -march=core2 -g -fopenmp -ffast-math")
	#set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}")
	#if (WIN32)
	#	set(CMAKE_BUILD_TYPE RELEASE)
	#	set(CMAKE_CXX_FLAGS "/Ox /GL /openmp /EHr /EHsc")
	#	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /TC /Ox /GL /openmp")
	#	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
	#endif()

#find_package(PkgConfig REQUIRED)

# Require CURL (for telemetry)
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
link_libraries(${CURL_LIBRARIES})

# Require Lua
find_package(Lua 5.3 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
#link_libraries(${LUA_LIBRARIES})

# Require GLEW
find_package(Glew REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# Require GLFW
find_package(glfw3 REQUIRED)
#find_package(glfw3 REQUIRED)
#include_directories(${GLFW_INCLUDE_DIRS})
#link_libraries(${GLFW_LIBRARIES})

# Require OpenGL
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})

# Require GLM
find_package(Glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Require Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})
link_libraries(${Boost_system_LIBRARY})
link_libraries(${Boost_filesystem_LIBRARY})

# Cereal
find_package(cereal REQUIRED)
include_directories(${CEREAL_INCLUDE_DIR})

# Zlib
find_package(zlib REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
link_libraries(${ZLIB_LIBRARIES})

#GLM
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIR})
link_libraries(${GLM_LIBRARIES})

# Include the main src folder, to keep stdafx happy
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(bengine)
add_subdirectory(components)
add_subdirectory(global_assets)
add_subdirectory(main_loops)
add_subdirectory(planet)
add_subdirectory(raws)
add_subdirectory(render_engine)
add_subdirectory(systems)

add_executable(noxfutura stdafx.cpp main.cpp)
target_link_libraries(noxfutura ${Boost_LIBRARIES} bengine mainloops globalassets raws planet components render_engine glfw ${LUA_LIBRARIES})

#set_target_properties(noxfutura PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
#cotire(noxfutura)