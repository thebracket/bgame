#include "sentient_ai_system.hpp"
#include "../../messages/messages.hpp"
#include "../../main/game_globals.hpp"
#include "../damage/weapons_helpers.hpp"
#include "../path_finding.hpp"
#include "../tasks/pathfinding.hpp"
#include "../tasks/initiative.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/position.hpp"
#include "../../components/viewshed.hpp"
#include "../../components/health.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/species.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/species.hpp"
#include "../tasks/threat_scanner.hpp"

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
                bool vegetarian = false;                    
                auto finder = get_species_def(planet.civs.unimportant_people[ai.person_id].species_tag).get();
                if (finder.diet == diet_herbivore) vegetarian = true;

                auto hostile = tasks::can_see_hostile(e, pos, view, [&vegetarian, &ai] (entity_t &other) {
                    bool hostile_sentient = false;
                    auto other_sentient = other.component<sentient_ai>();
                    if (other_sentient) {
                        const std::size_t my_civ = planet.civs.unimportant_people[ai.person_id].civ_id;
                        const std::size_t their_civ = planet.civs.unimportant_people[other_sentient->person_id].civ_id;
                        if (my_civ != their_civ) {
                            auto civfinder = planet.civs.civs[my_civ].relations.find(their_civ);
                            if (civfinder != planet.civs.civs[my_civ].relations.end()) {
                                if (civfinder->second < 0) hostile_sentient = true;
                            }
                        } 
                    }

                    if ((other.component<grazer_ai>() && !vegetarian) || 
                        (other.component<settler_ai_t>() && ai.hostile) ||
                        hostile_sentient) 
                    {
                        return true;
                    }

                    return false;
                });

                if (hostile.terrified) {
					// Run away! Eventually, we want the option for combat here based on morale. Also, when hunting
					// is implemented it's a good idea not to run away from your target.
                    const float range = shooting_range(e, pos);
					if (hostile.terror_distance < 1.5F) {
						// Hit it with melee weapon
						emit_deferred(settler_attack_message{e.id, hostile.closest_fear});
                        initiative_penalty += get_weapon_initiative_penalty(get_melee_id(e));
					} else if ( range != -1 && range < hostile.terror_distance) {
						// Shoot it
						emit_deferred(settler_ranged_attack_message{e.id, hostile.closest_fear});
                        initiative_penalty += get_weapon_initiative_penalty(get_ranged_and_ammo_id(e).first);
					} else {
                        emit_deferred(entity_wants_to_charge_message{e.id, hostile.closest_fear});
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
                                ai.current_path = find_path(pos, *entity(it->second)->component<position_t>(), false, planet.civs.unimportant_people[ai.person_id].civ_id);
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
