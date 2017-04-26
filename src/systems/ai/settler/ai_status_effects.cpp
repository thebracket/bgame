#include "ai_status_effects.hpp"
#include "../../../components/ai_tag_my_turn.hpp"
#include "../../../components/health.hpp"
#include "../../../main/game_pause.hpp"

void ai_status_effects::configure() {}

void ai_status_effects::update(const double duration_ms) {
    if (pause_mode != RUNNING) return;

    each<health_t, ai_tag_my_turn_t>([] (entity_t &e, health_t &health, ai_tag_my_turn_t &t) {
        // If unsconscious, cancel the turn
        if (health.unconscious) {
            delete_component<ai_tag_my_turn_t>(e.id);
        }
    });
}