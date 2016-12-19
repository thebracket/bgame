#include "healing_system.hpp"
#include "../../messages/messages.hpp"
#include "../../components/health.hpp"

using namespace rltk;

void healing_system::on_message(const hour_elapsed_message &msg) {
    each<health_t>([] (entity_t &e, health_t &h) {
        if (h.max_hitpoints > h.current_hitpoints) {
            ++h.current_hitpoints;
            h.unconscious = false;
        }
    });
}
