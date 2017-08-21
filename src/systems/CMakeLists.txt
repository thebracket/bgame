add_library(systems run_systems.cpp)
add_subdirectory(io)
target_link_libraries(systems io)
cotire(systems)
