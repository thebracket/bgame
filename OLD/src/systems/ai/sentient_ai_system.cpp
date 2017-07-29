#include "sentient_ai_system.hpp"
#include "../../messages/messages.hpp"
#include "../tasks/pathfinding.hpp"
#include "../tasks/initiative.hpp"
#include "../../components/sentient_ai.hpp"
#include "../tasks/threat_scanner.hpp"

using tasks::calculate_initiative;

void sentient_ai_system::configure() {

    subscribe<day_elapsed_message>([] (day_elapsed_message &msg) {
        each<sentient_ai>([] (entity_t &e, sentient_ai &ai) {
            ++ai.days_since_arrival;
        });
    });

    subscribe_mbox<action_available_message>();
}

void sentient_ai_system::update(const double ms) {}
