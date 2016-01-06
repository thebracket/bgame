#pragma once

#include "../engine/engine.hpp"
#include "components/components.h"
#include "messages/messages.h"

using engine::message_list_t;
using engine::component_list_t;

using my_messages = message_list_t<command_message, power_consumed_message, chat_emote_message,
mouse_motion_message, build_order_message, item_changed_message, walkability_changed_message,
entity_moved_message, chop_order_message, reaction_order_message, inflict_damage_message,
particle_message, highlight_message>;

using my_components = component_list_t<calendar_component, debug_name_component, obstruction_component,
power_battery_component, power_generator_component, renderable_component, settler_ai_component,
viewshed_component, game_stats_component, game_species_component, game_health_component, provisions_component,
item_storage_component, item_carried_component, description_component, item_component, tree_component,
particle_emitter_component, position_component3d, walkable_roof_component, point_light_component>;

using my_engine = engine::bracket_engine<my_components, my_messages>;

extern my_engine * game_engine;

void run_game();

// For convenient access
#define ECS game_engine->ecs
