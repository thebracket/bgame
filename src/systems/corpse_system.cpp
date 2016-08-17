#include "corpse_system.hpp"
#include "../components/components.hpp"
#include "../messages/messages.hpp"

void corpse_system::configure() {
    system_name = "Corpse Handler";
	subscribe<tick_message>([this](tick_message &msg) {
        std::vector<std::size_t> to_destroy;

        each<corpse_settler>([&to_destroy] (entity_t &e, corpse_settler &corpse) {
            ++corpse.ticks_since_death;
            if (corpse.ticks_since_death > 5000) {
                // TODO: Emit decomposition
            }
            if (corpse.ticks_since_death > 10000) {
                to_destroy.push_back(e.id);
            }
        });
        each<corpse_harvestable>([&to_destroy] (entity_t &e, corpse_harvestable &corpse) {
            ++corpse.ticks_since_death;
            if (corpse.ticks_since_death > 5000) {
                // TODO: Emit decomposition
            }
            if (corpse.ticks_since_death > 10000) {
                to_destroy.push_back(e.id);
            }
        });

        for (const std::size_t &id : to_destroy) {
            delete_entity(id);
        }
    });    
}

void corpse_system::update(const double ms) {

}
