add_library(physics
        door_system.cpp
        explosive_system.cpp
        fluid_system.cpp
        gravity_system.cpp
        movement_system.cpp
        topology_system.cpp
        trigger_system.cpp
        vegetation_system.cpp
        visibility_system.cpp
        )
target_link_libraries(physics ai )
