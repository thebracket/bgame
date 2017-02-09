#include "ai_new_arrival.hpp"
#include "../../main/game_globals.hpp"
#include "../../components/ai_settler_new_arrival.hpp"
#include "../../components/ai_mode_idle.hpp"
#include "../../components/ai_tag_my_turn.hpp"
#include "../../messages/log_message.hpp"
#include "../../raws/string_table.hpp"

void ai_new_arrival::configure() {}

void ai_new_arrival::update(const double duration_ms) {
    if (pause_mode != RUNNING) return;

    each<ai_tag_my_turn_t, ai_settler_new_arrival_t>([] (entity_t &e, ai_tag_my_turn_t &t, ai_settler_new_arrival_t &arrival) {
        delete_component<ai_tag_my_turn_t>(e.id); // No more turns

        if (arrival.turns_since_arrival == 0 && rng.roll_dice(1,6)==1) {
            const std::string quip = new_arrival_quips.random_entry(rng);
            emit(log_message{ LOG{}.settler_name(e.id)->text("says ")->col(rltk::colors::YELLOW)->text(quip)->chars });
        }

        ++arrival.turns_since_arrival;
        if (arrival.turns_since_arrival > 50) {
            delete_component<ai_settler_new_arrival_t>(e.id);
            e.assign(ai_mode_idle_t{});
        }
    });
}