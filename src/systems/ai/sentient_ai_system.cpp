#include "sentient_ai_system.hpp"
#include "../../messages/messages.hpp"
#include "../../main/game_globals.hpp"
#include "../damage/weapons_helpers.hpp"
#include "path_finding.hpp"
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
#include "../../components/initiative.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/species.hpp"
#include "../tasks/threat_scanner.hpp"
#include "distance_map_system.hpp"

using tasks::calculate_initiative;

void sentient_ai_system::configure() {

    subscribe<day_elapsed_message>([] (day_elapsed_message &msg) {
        each<sentient_ai>([] (entity_t &e, sentient_ai &ai) {
            ++ai.days_since_arrival;
        });
    });

    subscribe_mbox<action_available_message>();
}

void sentient_ai_system::update(const double ms) {
    each_mbox<action_available_message>([this] (const action_available_message &msg) {
        auto e = entity(msg.entity_id);
        auto ai = e->component<sentient_ai>();
        if (!ai) return;
        auto pos = e->component<position_t>();
        auto view = e->component<viewshed_t>();
        auto health = e->component<health_t>();
        auto stats = e->component<game_stats_t>();
        auto initiative = e->component<initiative_t>();
        if (!pos || !view || !health || !stats || !initiative) return;

        if (health->unconscious) {
            return;
        }

        // How do we feel about the Cordex civ?
        int feelings = planet.civs.civs[planet.civs.unimportant_people[ai->person_id].civ_id].cordex_feelings;
        if (feelings < 0) {
            ai->hostile = true;
        } else {
            ai->hostile = false;
        }

        // Look for immediate threats
        bool vegetarian = false;
        auto finder = *get_species_def(planet.civs.unimportant_people[ai->person_id].species_tag);
        if (finder.diet == diet_herbivore) vegetarian = true;

        auto hostile = tasks::can_see_hostile(*e, *pos, *view, [&vegetarian, &ai] (entity_t &other) {
            bool hostile_sentient = false;
            auto other_sentient = other.component<sentient_ai>();
            if (other_sentient) {
                const std::size_t my_civ = planet.civs.unimportant_people[ai->person_id].civ_id;
                const std::size_t their_civ = planet.civs.unimportant_people[other_sentient->person_id].civ_id;
                if (my_civ != their_civ) {
                    auto civfinder = planet.civs.civs[my_civ].relations.find(their_civ);
                    if (civfinder != planet.civs.civs[my_civ].relations.end()) {
                        if (civfinder->second < 0) hostile_sentient = true;
                    }
                }
            }

            if ((other.component<grazer_ai>() && !vegetarian) ||
                (other.component<settler_ai_t>() && ai->hostile) ||
                hostile_sentient)
            {
                return true;
            }

            return false;
        });

        if (hostile.terrified) {
            // Run away! Eventually, we want the option for combat here based on morale. Also, when hunting
            // is implemented it's a good idea not to run away from your target.
            const float range = shooting_range(*e, *pos);
            if (hostile.terror_distance < 1.5F) {
                // Hit it with melee weapon
                emit_deferred(settler_attack_message{e->id, hostile.closest_fear});
                initiative->initiative_modifier += get_weapon_initiative_penalty(get_melee_id(*e));
            } else if ( range != -1 && range < hostile.terror_distance) {
                // Shoot it
                emit_deferred(settler_ranged_attack_message{e->id, hostile.closest_fear});
                initiative->initiative_modifier += get_weapon_initiative_penalty(get_ranged_and_ammo_id(*e).first);
            } else {
                emit_deferred(entity_wants_to_charge_message{e->id, hostile.closest_fear});
                ai->goal = SENTIENT_GOAL_CHARGE;
            }
        } else {
            if (ai->goal == SENTIENT_GOAL_CHARGE || ai->goal == SENTIENT_GOAL_FLEE) {
                ai->goal = SENTIENT_GOAL_IDLE;
            }

            if (ai->goal == SENTIENT_GOAL_IDLE && ai->hostile && rng.roll_dice(1,500)-1+(0-feelings) <= ai->aggression && ai->days_since_arrival > 1) {
                // Can we get to a settler?
                const int idx = mapidx(*pos);
                const int distance = settler_map.distance_map[idx];
                if (distance >= MAX_DIJSTRA_DISTANCE) {
                    // Nobody to attack!
                    ai->goal = SENTIENT_GOAL_IDLE;
                } else {
                    // Close for the kill...
                    ai->goal = SENTIENT_GOAL_KILL;
                }
            } else if (ai->goal == SENTIENT_GOAL_KILL ) {
                const int idx = mapidx(*pos);
                const int distance = settler_map.distance_map[idx];
                if (distance >= MAX_DIJSTRA_DISTANCE) {
                    // Nobody to attack!
                    ai->goal = SENTIENT_GOAL_IDLE;
                } else if (distance < 1) {
                    // We've arrived - the combat code should kick in on its own if applicable
                    ai->goal = SENTIENT_GOAL_IDLE;
                } else {
                    // Close for the kill...
                    position_t destination = settler_map.find_destination(*pos);
                    emit_deferred(entity_wants_to_move_message{e->id, destination});
                    emit_deferred(renderables_changed_message{});
                }
                return;
            } else if (ai->goal == SENTIENT_GOAL_IDLE) {
                // Otherwise we move randomly
                emit_deferred(entity_wants_to_move_randomly_message{e->id});
            }
        }
    });
}
