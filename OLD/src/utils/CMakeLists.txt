add_subdirectory(gl)
add_library(utils STATIC
    string_utils.cpp
	octree.cpp
    dijkstra_map.cpp
	telemetry.cpp
	filesystem.cpp
)
cotire(utils)
