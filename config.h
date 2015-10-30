#pragma once

#define GAME engine::bracket_engine<engine::sdl2_backend>
#define GAME_ECS entity_component_system<engine::ecs::calendar_component, engine::ecs::debug_name_component, engine::ecs::obstruction_component, engine::ecs::position_component, engine::ecs::power_battery_component, engine::ecs::power_generator_component, engine::ecs::renderable_component, engine::ecs::settler_ai_component, engine::ecs::viewshed_component, engine::ecs::game_stats_component>
