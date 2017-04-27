#include "ai_action_shim.hpp"
#include "../../../components/ai_tags/ai_mode_idle.hpp"
#include "../../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../../components/ai_tags/ai_tag_work_shift.hpp"
#include "../../../messages/tick_message.hpp"
#include "../../../main/game_pause.hpp"

void ai_action_shim::configure() {}

void ai_action_shim::update(const double duration_ms) {
    if (pause_mode != RUNNING) return;

    each<ai_tag_my_turn_t, ai_mode_idle_t>([] (entity_t &e, ai_tag_my_turn_t &t, ai_mode_idle_t &i) {
        emit(action_available_message{e.id});
        delete_component<ai_tag_my_turn_t>(e.id);
    });

    each<ai_tag_my_turn_t, ai_tag_work_shift_t>([] (entity_t &e, ai_tag_my_turn_t &t, ai_tag_work_shift_t &i) {
        emit(action_available_message{e.id});
        delete_component<ai_tag_my_turn_t>(e.id);
        delete_component<ai_tag_work_shift_t>(e.id);
        e.assign(ai_mode_idle_t{});
    });
}
