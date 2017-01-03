#include "distance_map_system.hpp"
#include "../messages/tick_message.hpp"
#include "../components/grazer_ai.hpp"
#include "../components/position.hpp"
#include "../planet/region.hpp"
#include "../messages/entity_moved_message.hpp"

dijkstra_map huntables_map;

using namespace rltk;

void distance_map_system::configure() {
    system_name = "Distance Maps";
    subscribe_mbox<huntable_moved_message>();
}

void distance_map_system::update(const double duration_ms) {
    each_mbox<huntable_moved_message>([this] (const huntable_moved_message &msg) { update_huntables = true; });

    if (update_huntables) {
        std::vector<int> huntables;
        each<grazer_ai, position_t>([&huntables] (entity_t &e, grazer_ai &ai, position_t &pos) {
            huntables.emplace_back(mapidx(pos));
        });
        huntables_map.update(huntables);

        update_huntables = false;
    }
}