#include "sentient_ai_system.hpp"
#include "../components/components.hpp"
#include "../messages/messages.hpp"
#include "../main/game_globals.hpp"
#include "weapons_helpers.hpp"

void sentient_ai_system::configure() {
    system_name = "Sentient AI";

    subscribe<tick_message>([this](tick_message &msg) {
        parallel_each<sentient_ai, position_t, viewshed_t>([] (entity_t &e, sentient_ai &ai, position_t &pos, viewshed_t &view) {
            if (ai.initiative < 1) {
                // How do we feel about the Cordex civ?
                int feelings = planet.civs.civs[planet.civs.unimportant_people[ai.person_id].civ_id].cordex_feelings;
                if (feelings < 0) {
                    ai.hostile = true;
                } else {
                    ai.hostile = false;
                }
                //std::cout << "Feelings: " << feelings << "\n";

                // Look for immediate threats
                bool terrified = false;
                float terror_distance = 1000.0F;
                std::size_t closest_fear = 0;
                for (const std::size_t other_entity : view.visible_entities) {
					entity_t * other_ptr = rltk::entity(other_entity);
                    if (other_ptr != nullptr && (other_ptr->component<grazer_ai>() != nullptr || 
						(other_ptr->component<settler_ai_t>() != nullptr && ai.hostile))) {
                        terrified = true;
                        position_t * other_pos = rltk::entity(other_entity)->component<position_t>();
                        const float d = distance3d(pos.x, pos.y, pos.z, other_pos->x, other_pos->y, other_pos->z);
                        if (d < terror_distance) {
                            terror_distance = d;
                            closest_fear = other_entity;
                        }
                    }
                }

                if (terrified) {
					// Run away! Eventually, we want the option for combat here based on morale. Also, when hunting
					// is implemented it's a good idea not to run away from your target.
                    const float range = shooting_range(e, pos);
                    std::cout << range << "\n";
					if (terror_distance < 1.5F) {
						// Hit it with melee weapon
						emit(settler_attack_message{e.id, closest_fear});
                        //std::cout << "Attack!\n";
					} else if ( range != -1 && range < terror_distance) {
						// Shoot it
						emit(settler_ranged_attack_message{e.id, closest_fear});
                        //std::cout << "Shoot!\n";
					} else {
                        if (rng.roll_dice(1,10) > ai.aggression) {
                            emit(entity_wants_to_flee_message{e.id, closest_fear});
                            //std::cout << "Flee!\n";
                        } else {
                            emit(entity_wants_to_charge_message{e.id, closest_fear});
                            //std::cout << "Charge!\n";
                        }
					}
				} else {
                    // Otherwise we move randomly
                    emit_deferred(entity_wants_to_move_randomly_message{e.id});
                }

                ai.initiative = std::max(1, rng.roll_dice(1, 12) - ai.initiative_modifier);
            } else {
                --ai.initiative;
            }
        });
    });
}

void sentient_ai_system::update(const double ms) {

}
