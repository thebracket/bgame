#include "sentient_ai_system.hpp"
#include "../../messages/messages.hpp"
#include "../../main/game_globals.hpp"
#include "../damage/weapons_helpers.hpp"
#include "path_finding.hpp"
#include "../tasks/pathfinding.hpp"
#include "../tasks/initiative.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/position.hpp"
#include "../../components/viewshed.hpp"
#include "../../components/health.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/species.hpp"
#include "../../components/initiative.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/species.hpp"
#include "../tasks/threat_scanner.hpp"
#include "distance_map_system.hpp"

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
