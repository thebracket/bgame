#include "ai_idle.hpp"
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
#include "../../raws/species.hpp"
#include "../../main/game_planet.hpp"
#include "../../main/game_pause.hpp"
#include "../../main/game_region.hpp"
#include "../../main/game_rng.hpp"

void ai_idle::configure() {}

void idle_grazer(entity_t &e, ai_tag_my_turn_t &t, ai_mode_idle_t &idle, grazer_ai * grazer) {
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
}

void idle_sentient(entity_t &e, ai_tag_my_turn_t &t, ai_mode_idle_t &idle, sentient_ai * sentient) {
    auto pos = e.component<position_t>();
    int feelings = planet.civs.civs[sentient->civ_id].cordex_feelings;

    if (sentient->hostile || feelings < 0) {
        sentient->goal = SENTIENT_GOAL_KILL;
    } else {
        sentient->goal = SENTIENT_GOAL_IDLE;
    }

    if (sentient->goal == SENTIENT_GOAL_KILL) {
        //std::cout << "Sentient kill mode\n";
        // Close for the kill!
        const int idx = mapidx(*pos);
        if (settler_map.get(idx) < MAX_DIJSTRA_DISTANCE-1) {
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
}

void idle_iterator(entity_t &e, ai_tag_my_turn_t &t, ai_mode_idle_t &idle) {
    auto settler = e.component<settler_ai_t>();
    auto sentient = e.component<sentient_ai>();
    auto grazer = e.component<grazer_ai>();

    if (grazer) {
        idle_grazer(e, t, idle, grazer);
    } else if (sentient) {
        idle_sentient(e, t, idle, sentient);
    } else if (settler) {
        // TODO
    }
}

void ai_idle::update(const double duration_ms) {
    if (pause_mode != RUNNING) return;

    each<ai_tag_my_turn_t, ai_mode_idle_t>(idle_iterator);
}