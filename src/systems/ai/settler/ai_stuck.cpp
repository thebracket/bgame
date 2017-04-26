#include "ai_stuck.hpp"
#include "../../tasks/pathfinding.hpp"
#include "../../../messages/log_message.hpp"
#include "../../../components/ai_tag_my_turn.hpp"
#include "../../../components/construct_provides_sleep.hpp"
#include "../../tasks/settler_move_to.hpp"

void ai_stuck_system::configure() {
    system_name = "AI Stuck";
}

void ai_stuck_system::update(const double duration_ms) {
    each<ai_tag_my_turn_t, position_t, settler_ai_t>([this] (entity_t &e, ai_tag_my_turn_t &turn, position_t &pos, settler_ai_t &ai) {
        if (tasks::is_stuck_or_invalid(pos)) {
            emit_deferred(log_message{LOG{}.text("Warning - settler is stuck; activating emergency teleport to bed!")->chars});
            std::cout << "Warning - stuck settler!\n";
            bool done = false;
            each<position_t, construct_provides_sleep_t>([this,&e,&pos,&done] (entity_t &E, position_t &P, construct_provides_sleep_t &S) {
                if (!done) {
                    move_to(e, pos, P);
                    done = true;
                    // This should use power
                }
            });
            delete_component<ai_tag_my_turn_t>(e.id);
        }
    });
}