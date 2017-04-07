#include "ai_leisure_time.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/ai_tag_leisure_shift.hpp"
#include "../../components/ai_tag_my_turn.hpp"
#include "../../messages/entity_moved_message.hpp"
#include "../../components/ai_mode_idle.hpp"

void ai_leisure_time_t::configure() {

}

void ai_leisure_time_t::update(const double duration_ms) {
    each<settler_ai_t, ai_tag_leisure_shift_t>([] (entity_t &e, settler_ai_t &ai, ai_tag_leisure_shift_t &leisure) {
        delete_component<ai_tag_leisure_shift_t>(e.id);
        delete_component<ai_tag_my_turn_t>(e.id);
        e.assign(ai_mode_idle_t{});

        // We don't have any leisure activities yet, so move randomly.
        emit_deferred(entity_wants_to_move_randomly_message{e.id});
    });
}