add_library(physics STATIC
        fluid_system.cpp
        gravity_system.cpp
        )
target_link_libraries(physics rltk utils components raws)
cotire(physics)
