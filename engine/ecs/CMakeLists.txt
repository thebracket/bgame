add_library(ecs ecs.cpp entity_factory.cpp entity.cpp systems/calendar_system.cpp systems/camera_system.cpp systems/renderable_system.cpp systems/settler_ai_system.cpp systems/viewshed_system.cpp components/base_component.cpp systems/obstruction_system.cpp systems/power_system.cpp)

add_subdirectory(systems)
add_subdirectory(components)