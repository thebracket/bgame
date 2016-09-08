#include "sentient_ai_system.hpp"
#include "../components/components.hpp"
#include "../messages/messages.hpp"

void sentient_ai_system::configure() {
    system_name = "Sentient AI";

    subscribe<tick_message>([this](tick_message &msg) {
        parallel_each<sentient_ai, position_t>([] (entity_t &e, sentient_ai &ai, position_t &pos) {
            if (ai.initiative < 1) {
                emit_deferred(entity_wants_to_move_randomly_message{e.id});

                ai.initiative = std::max(1, rng.roll_dice(1, 12) - ai.initiative_modifier);
            } else {
                --ai.initiative;
            }
        });
    });
}

void sentient_ai_system::update(const double ms) {

}
