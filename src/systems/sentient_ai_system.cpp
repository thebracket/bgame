#include "sentient_ai_system.hpp"
#include "../components/components.hpp"
#include "../messages/messages.hpp"
#include "../main/game_globals.hpp"
#include "weapons_helpers.hpp"
#include "path_finding.hpp"
#include "tasks/pathfinding.hpp"
#include "tasks/initiative.hpp"

using tasks::calculate_initiative;

void sentient_ai_system::configure() {
    system_name = "Sentient AI";

    subscribe<day_elapsed_message>([] (day_elapsed_message &msg) {
        each<sentient_ai>([] (entity_t &e, sentient_ai &ai) {
            ++ai.days_since_arrival;
        });
    });

    subscribe_mbox<tick_message>();
}

void sentient_ai_system::update(const double ms) {
    std::queue<tick_message> * ticks = mbox<tick_message>();
    while (!ticks->empty()) {
        tick_message msg = ticks->front();
        ticks->pop();

        each<sentient_ai, position_t, viewshed_t, health_t, game_stats_t>([] (entity_t &e, sentient_ai &ai, position_t &pos, viewshed_t &view, health_t &health, game_stats_t &stats) {
            int initiative_penalty = 0 - ai.initiative_modifier;

            if (ai.initiative < 1) {
                if (health.unconscious) {
                    calculate_initiative(ai, stats, initiative_penalty);
                    return;
                }

                // How do we feel about the Cordex civ?
                int feelings = planet.civs.civs[planet.civs.unimportant_people[ai.person_id].civ_id].cordex_feelings;
                if (feelings < 0) {
                    ai.hostile = true;
                } else {
                    ai.hostile = false;
                }

                // Look for immediate threats
                bool terrified = false;
                float terror_distance = 1000.0F;
                std::size_t closest_fear = 0;
                for (const std::size_t other_entity : view.visible_entities) {
                    bool vegetarian = false;
                    auto finder = species_defs.find(planet.civs.civs[planet.civs.unimportant_people[ai.person_id].civ_id].species_tag);
                    if (finder != species_defs.end() && finder->second.diet == diet_herbivore) vegetarian = true;
					auto other_ptr = rltk::entity(other_entity);
                    if ((other_ptr && (other_ptr->component<grazer_ai>() && !vegetarian) || 
						(other_ptr->component<settler_ai_t>() && ai.hostile))) {
                        terrified = true;
                        auto other_pos = rltk::entity(other_entity)->component<position_t>();
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
					if (terror_distance < 1.5F) {
						// Hit it with melee weapon
						emit_deferred(settler_attack_message{e.id, closest_fear});
                        initiative_penalty += get_weapon_initiative_penalty(get_melee_id(e));
					} else if ( range != -1 && range < terror_distance) {
						// Shoot it
						emit_deferred(settler_ranged_attack_message{e.id, closest_fear});
                        initiative_penalty += get_weapon_initiative_penalty(get_ranged_and_ammo_id(e).first);
					} else {
                        emit_deferred(entity_wants_to_charge_message{e.id, closest_fear});
                        ai.goal = SENTIENT_GOAL_CHARGE;
					}
				} else {
                    if (ai.goal == SENTIENT_GOAL_CHARGE || ai.goal == SENTIENT_GOAL_FLEE) {
                        ai.goal = SENTIENT_GOAL_IDLE;
                    }

                    if (ai.goal == SENTIENT_GOAL_IDLE && ai.hostile && rng.roll_dice(1,500)-1+(0-feelings) <= ai.aggression && ai.days_since_arrival > 1) {
                        // Look for a settler to kill
                        std::map<float, std::size_t> targets;
                        each<settler_ai_t, position_t>([&targets, &pos] (entity_t &se, settler_ai_t &settler_ai, position_t &spos) {
                            const float distance = distance3d(pos.x, pos.y, pos.z, spos.x, spos.y, spos.z);
                            targets[distance] = se.id; 
                        });

                        if (!targets.empty()) {
                            auto it = targets.begin();
                            ai.target = 0;
                            while (ai.target == 0 && it != targets.end()) {
                                ai.current_path = find_path(pos, *entity(it->second)->component<position_t>());
                                if (ai.current_path->success) {
                                    ai.target = it->second;
                                }

                                ++it;
                            }

                            if (ai.target == 0) {
                                ai.current_path.reset();
                            } else {
                                ai.goal = SENTIENT_GOAL_KILL;
                            }
                        }
                    } else if (ai.goal == SENTIENT_GOAL_KILL ) {
                        tasks::try_path(e, ai, pos,
                            [] () {}, // Nothing on success
                            [&ai] () {
                                ai.current_path.reset();
                                ai.goal = SENTIENT_GOAL_IDLE;
                                ai.target = 0;
                                ai.initiative = std::max(1, rng.roll_dice(1, 12) - ai.initiative_modifier);
                            }, // On arrival
                            [&ai] () {
                                ai.current_path.reset();
                                ai.goal = SENTIENT_GOAL_IDLE;
                                ai.target = 0;
                                ai.initiative = std::max(1, rng.roll_dice(1, 12) - ai.initiative_modifier);
                            } // On fail
                        );
                        return;
                    } else if (ai.goal == SENTIENT_GOAL_IDLE) {
                        // Otherwise we move randomly
                        emit_deferred(entity_wants_to_move_randomly_message{e.id});
                    }
                }

                calculate_initiative(ai, stats, initiative_penalty);
            } else {
                --ai.initiative;
            }
        });
    }
}
