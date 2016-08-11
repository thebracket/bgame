#include "mode_rogue_render_system.hpp"

#include "../game_globals.hpp"
#include <rltk.hpp>

void mode_rogue_render_system::configure() {
    system_name = "Rogue Render System";
}

void mode_rogue_render_system::update(const double ms) {
    if (game_master_mode != ROGUE) return;
    
    // Draw the current settlers info panel
    entity_t * e = entity(selected_settler);
    if (e == nullptr) {
        // The settler has died!
        game_master_mode = PLAY;
        pause_mode = PAUSED;
        return;
    }
    name_t * name = e->component<name_t>();
	game_stats_t * stats = e->component<game_stats_t>();
	species_t * species = e->component<species_t>();
	settler_ai_t * ai = e->component<settler_ai_t>();
	health_t * health = e->component<health_t>(); 

    int y=5;
    term(2)->print(3, y, "Now controlling:"); ++y;
    term(2)->print(3, y, name->first_name + std::string(" ") + name->last_name); ++y;
    term(2)->print(3, y, std::to_string(health->current_hitpoints) + std::string("/") + std::to_string(health->max_hitpoints) + std::string(" hp")); ++y;

    if (health->unconscious) {
        term(2)->print(3, y, "UNCONSCIOUS!", rltk::colors::RED); ++y;
    }

    term(2)->print(3, y, "Click on map to move/interact"); ++y;
    if (ai->targeted_hostile != 0) {
        name_t * target_name = entity(ai->targeted_hostile)->component<name_t>();
        term(2)->print(3, y, "Click now to attack " + target_name->first_name); ++y;
    }          
}