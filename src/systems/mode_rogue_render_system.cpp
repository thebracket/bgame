#include "mode_rogue_render_system.hpp"

#include "../main/game_globals.hpp"
#include <rltk.hpp>
#include "weapons_helpers.hpp"
#include "../components/health.hpp"
#include "../components/settler_ai.hpp"
#include "../components/grazer_ai.hpp"
#include "../components/sentient_ai.hpp"
#include "../components/species.hpp"
#include "../components/position.hpp"
#include "../components/game_stats.hpp"

void mode_rogue_render_system::configure() {
    system_name = "Rogue Render System";
}

void mode_rogue_render_system::update(const double ms) {
    if (game_master_mode != ROGUE) return;
    
    // Draw the current settlers info panel
    auto e = entity(selected_settler);
    if (!e) {
        // The settler has died!
        game_master_mode = PLAY;
        pause_mode = PAUSED;
        return;
    }
    auto name = e->component<name_t>();
	auto stats = e->component<game_stats_t>();
	auto species = e->component<species_t>();
	auto ai = e->component<settler_ai_t>();
	auto health = e->component<health_t>(); 
    auto pos = e->component<position_t>();

    int y=5;
    term(2)->print(3, y, "Now controlling:"); ++y;
    term(2)->print(3, y, name->first_name + std::string(" ") + name->last_name); ++y;
    term(2)->print(3, y, std::to_string(health->current_hitpoints) + std::string("/") + std::to_string(health->max_hitpoints) + std::string(" hp")); ++y;

    if (health->unconscious) {
        term(2)->print(3, y, "UNCONSCIOUS!", rltk::colors::RED); ++y;
    }

    term(2)->print(3, y, "Click on map to move/interact"); ++y;
    if (ai->targeted_hostile != 0) {
        auto hostile = entity(ai->targeted_hostile);
        
        if (hostile) {
            auto hostile_pos = hostile->component<position_t>();
            auto target_name = hostile->component<name_t>();
            const float distance = distance3d(pos->x, pos->y, pos->z, hostile_pos->x, hostile_pos->y, hostile_pos->z);
            if (distance < 1.5F) {
                if (has_melee_weapon(*e)) {
                    term(2)->print(3, y, "Click now to melee attack " + target_name->first_name); 
                    ++y;
                } else {
                    term(2)->print(3, y, "Click now to punch " + target_name->first_name); 
                    ++y;
                }
                ai->current_path.reset();
            } else {
                auto ranged = has_ranged_weapon(*e);
                if (ranged.first && has_appropriate_ammo(*e, ranged.second, *pos) && distance < shooting_range(*e, *hostile_pos)) {
                    term(2)->print(3, y, "Click now to fire at " + target_name->first_name); 
                    ++y;
                } else {
                    term(2)->print(3, y, "If you had a ranged weapon and ammo, and were in range, you could fire at " + target_name->first_name); 
                    ++y;
                }
            }
        } else {
            ai->targeted_hostile = 0;
        }
    }          
}