#include "ai_idle.hpp"
#include "../../main/game_globals.hpp"
#include "../../components/ai_mode_idle.hpp"
#include "../../components/ai_tag_my_turn.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/position.hpp"
#include "../../messages/entity_moved_message.hpp"
#include "../../messages/vegetation_damage_message.hpp"
#include "distance_map_system.hpp"
#include "../../messages/renderables_changed_message.hpp"

void ai_idle::configure() {}

void ai_idle::update(const double duration_ms) {
    if (pause_mode != RUNNING) return;

    each<ai_tag_my_turn_t, ai_mode_idle_t>([] (entity_t &e, ai_tag_my_turn_t &t, ai_mode_idle_t &idle) {
        auto settler = e.component<settler_ai_t>();
        auto sentient = e.component<sentient_ai>();
        auto grazer = e.component<grazer_ai>();

        if (grazer) {
            auto pos = e.component<position_t>();
            delete_component<ai_tag_my_turn_t>(e.id);

            // Grazers simply eat vegetation or move
            const auto idx = mapidx(pos->x, pos->y, pos->z);
            if (current_region->tile_vegetation_type[idx] > 0) {
                if (rng.roll_dice(1,6)==1) emit_deferred(vegetation_damage_message{idx, 1});
            } else {
                emit_deferred(entity_wants_to_move_randomly_message{e.id});
                emit_deferred(huntable_moved_message{});
            }
            delete_component<ai_tag_my_turn_t>(e.id);
        } else if (sentient) {
            auto pos = e.component<position_t>();
            int feelings = 0;
            feelings = planet.civs.civs[planet.civs.population[sentient->person_id].civ_id].cordex_feelings;
            if (feelings < 0) {
                sentient->hostile = true;
            } else {
                sentient->hostile = false;
            }
            if (planet.civs.population[sentient->person_id].behavior == "eat_world") sentient->hostile = true;

            // There's a chance they will go berserk
            std::cout << planet.civs.population[sentient->person_id].behavior << "\n";
            if (planet.civs.population[sentient->person_id].behavior == "eat_world" ||
                    (sentient->goal == SENTIENT_GOAL_IDLE && rng.roll_dice(1,500)-1+(0-feelings) <= sentient->aggression && sentient->days_since_arrival > 1)) {
                const int idx = mapidx(*pos);
                //std::cout << "Kill mode detected, distance " << settler_map.distance_map[idx] << "\n";
                if (settler_map.distance_map[idx] < MAX_DIJSTRA_DISTANCE-1) {
                    sentient->goal = SENTIENT_GOAL_KILL;
                }
            }

            if (sentient->goal == SENTIENT_GOAL_KILL) {
                //std::cout << "Sentient kill mode\n";
                // Close for the kill!
                const int idx = mapidx(*pos);
                if (settler_map.distance_map[idx] < MAX_DIJSTRA_DISTANCE-1) {
                    position_t destination = settler_map.find_destination(*pos);
                    emit_deferred(entity_wants_to_move_message{e.id, destination});
                    emit_deferred(renderables_changed_message{});
                } else {
                    sentient->goal = SENTIENT_GOAL_IDLE;
                }
            } else {
                // Wander aimlessly
                emit_deferred(entity_wants_to_move_randomly_message{e.id});
            }
            delete_component<ai_tag_my_turn_t>(e.id);
        } else if (settler) {
            // TODO
        }
    });
}