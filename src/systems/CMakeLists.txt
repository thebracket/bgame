add_subdirectory(ai)
add_subdirectory(damage)
add_subdirectory(gui)
add_subdirectory(input)
add_subdirectory(physics)
add_subdirectory(tasks)
add_subdirectory(scheduler)
add_subdirectory(render)
add_library(systems STATIC
	power_system.cpp
	world_system.cpp
	stockpile_system.cpp
    systems.cpp
)
target_link_libraries(systems rltk utils components raws ai damage gui input physics tasks scheduler render)
cotire(systems)
