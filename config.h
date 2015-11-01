#pragma once

#define GAME engine::bracket_engine<engine::sdl2_backend>
#define GAME_ECS entity_component_system<calendar_component, debug_name_component, obstruction_component, position_component, power_battery_component, power_generator_component, renderable_component, settler_ai_component, viewshed_component, game_stats_component, game_species_component, game_health_component, provisions_component>
