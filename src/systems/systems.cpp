#include "systems.hpp"

#include "map_render_system.hpp"
#include "calendar_system.hpp"
#include "damage/settler_ai_system.hpp"
#include "panel_render_system.hpp"
#include "mining_system.hpp"
#include "inventory_system.hpp"
#include "visibility_system.hpp"
#include "workflow_system.hpp"
#include "topology_system.hpp"
#include "camera_system.hpp"
#include "renderables_system.hpp"
#include "lighting_system.hpp"
#include "power_system.hpp"
#include "damage/damage_system.hpp"
#include "wildlife_population_system.hpp"
#include "movement_system.hpp"
#include "gui/mode_units_system.hpp"
#include "gui/mode_rogue_system.hpp"
#include "gui/mode_rogue_render_system.hpp"
#include "corpse_system.hpp"
#include "particle_system.hpp"
#include "vegetation_system.hpp"
#include "keyboard_system.hpp"
#include "sentient_ai_system.hpp"
#include "world_system.hpp"
#include "gui/game_over_system.hpp"
#include "gui/mode_civs_system.hpp"
#include "log_system.hpp"
#include "mouse_input_system.hpp"
#include "gui/gui_system.hpp"
#include "gui/hud_system.hpp"
#include "gui/mode_settler_info_system.hpp"
#include "gui/mode_play_system.hpp"
#include "gui/mode_design_system.hpp"
#include "gui/mode_standing_orders.hpp"
#include "gui/mode_sentient_info_system.hpp"
#include "fluid_system.hpp"
#include "gravity_system.hpp"
#include "grazer_ai_system.hpp"
#include "gui/wish_system.hpp"
#include "door_system.hpp"
#include "pause_system.hpp"
#include "damage/settler_ranged_attack.hpp"
#include "damage/settler_melee_attacks.hpp"
#include "damage/creature_attacks.hpp"
#include "damage/kill_system.hpp"
#include "damage/healing_system.hpp"

void add_systems_to_ecs() {
    add_system<fluid_system>();
    add_system<keyboard_system>();
    add_system<mouse_input_system>();
    add_system<door_system>();
    add_system<pause_system>();
    add_system<calendar_system>();
    add_system<world_system>();
    add_system<gravity_system>();
    add_system<wildlife_population_system>();
    add_system<sentient_ai_system>();
    add_system<grazer_ai_system>();
    add_system<corpse_system>();
    add_system<particle_system>();
    add_system<renderables_system>();
    add_system<camera_system>();
    add_system<lighting_system>();
    add_system<mining_system>();
    add_system<inventory_system>();
    add_system<power_system>();
    add_system<workflow_system>();
    add_system<settler_ai_system>();
    add_system<mode_rogue_system>();
    add_system<movement_system>();
    add_system<settler_ranged_attack_system>();
    add_system<settler_melee_attacks_system>();
    add_system<creature_attacks_system>();
    add_system<damage_system>();
    add_system<kill_system>();
    add_system<healing_system>();
    add_system<topology_system>();
    add_system<visibility_system>();
    add_system<vegetation_system>();
    add_system<map_render_system>();
    add_system<hud_system>();
    add_system<panel_render_system>();
    add_system<mode_play_system>();
    add_system<mode_design_system>();
    add_system<mode_units_system>();
    add_system<mode_rogue_render_system>();
    add_system<mode_settler_info_system>();
    add_system<game_over_system>();
    add_system<mode_civs_system>();
    add_system<mode_standing_orders>();
    add_system<mode_sentient_info_system>();
    add_system<wish_system>();
    add_system<log_system>();
    add_system<gui_system>();
    add_system<fluid_system_end>();
}
