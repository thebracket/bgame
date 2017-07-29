cmake_minimum_required(VERSION 3.1)
project("Nox Futura")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake_modules)
include(cotire)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
	message("Using ccache")
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)
#if (NOT(WIN32))
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pipe -march=core2 -fomit-frame-pointer -mtune=native -pthread -g -fopenmp -ffast-math")
	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L/opt/local/lib -pthread")
	#endif()
	#if (WIN32)
	#	set(CMAKE_BUILD_TYPE RELEASE)
	#	set(CMAKE_CXX_FLAGS "/Ox /GL /openmp /EHr /EHsc")
	#	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /TC /Ox /GL /openmp")
	#	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
	#endif()

find_package(PkgConfig REQUIRED)

# Require GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# Require GLFW
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARIES})

# Require OpenGL
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})

# Require GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Require Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Cereal
find_package(cereal REQUIRED)
include_directories(${CEREAL_INCLUDE_DIR})

# Zlib
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
link_libraries(${ZLIB_LIBS})

add_subdirectory(bengine)
add_executable(noxfutura main.cpp)
target_link_libraries(noxfutura bengine)

cotire(noxfutura)