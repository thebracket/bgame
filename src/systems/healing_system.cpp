#include "healing_system.hpp"
#include "../messages/messages.hpp"
#include "../components/health.hpp"

using namespace rltk;

void healing_system::configure() {
    system_name = "Healing System";
    subscribe_mbox<hour_elapsed_message>();
}

void healing_system::heal_over_time() {
    std::queue<hour_elapsed_message> * hour = mbox<hour_elapsed_message>();
    while (!hour->empty()) {
        hour->pop();

        each<health_t>([] (entity_t &e, health_t &h) {
            if (h.max_hitpoints > h.current_hitpoints) {
                ++h.current_hitpoints;
                h.unconscious = false;
            }
        });
    }
}

void healing_system::update(const double ms) {
    heal_over_time();
}
