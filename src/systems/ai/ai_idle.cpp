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
        } else if (sentient) {
            // TODO
        } else if (settler) {
            // TODO
        }
    });
}