#include "stdafx.h"
#include "corpse_system.hpp"
#include "../../global_assets/rng.hpp"
#include <mutex>

namespace systems {
    namespace corpse_system {

		using namespace bengine;

        void run(const double &duration_ms) {
			std::vector<std::size_t> to_destroy;
			std::mutex destroy_guard;

			each<corpse_settler>([&to_destroy](entity_t &e, corpse_settler &corpse) {
				++corpse.ticks_since_death;
				if (corpse.ticks_since_death > 5000) {
					if (rng.roll_dice(1, 6) < 3) {
						auto pos = e.component<position_t>();
						// TODO: if (pos) emit_deferred(emit_particles_message{ 2, pos->x, pos->y, pos->z });
					}
				}
				if (corpse.ticks_since_death > 10000) {
					// TODO: Haunt someone!
				}
			});

			each<corpse_harvestable>([&to_destroy, &destroy_guard](entity_t &e, corpse_harvestable &corpse) {
				++corpse.ticks_since_death;
				if (corpse.ticks_since_death > 5000) {
					if (rng.roll_dice(1, 6) < 3) {
						auto pos = e.component<position_t>();
						// TODO: if (pos) emit_deferred(emit_particles_message{ 2, pos->x, pos->y, pos->z });
					}
				}
				if (corpse.ticks_since_death > 10000) {
					std::lock_guard<std::mutex> guard(destroy_guard);
					to_destroy.push_back(e.id);
				}
			});

			for (const std::size_t &id : to_destroy) {
				delete_entity(id);
			}
        }
    }
}
