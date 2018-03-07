#include "item_wear_system.hpp"
#include "../../bengine/ecs.hpp"
#include "../../components/items/item.hpp"
#include "../../components/items/item_wear.hpp"
#include "../../components/items/item_quality.hpp"
#include "../../global_assets/rng.hpp"
#include "../helpers/inventory_assistant.hpp"
#include "../../global_assets/game_ecs.hpp"

namespace systems {
	namespace item_wear {
		void run(const double &duration_ms) {
			using namespace bengine;

			// Runs once per day
			std::vector<std::size_t> to_destroy;
			each<item_t, item_wear_t, item_quality_t>([&to_destroy](entity_t &e, item_t &i, item_wear_t &w, item_quality_t &quality) {
				if (rng.roll_dice(1, 10) < quality.quality) {
					if (w.wear > 0) {
						--w.wear;
					}
					else {
						to_destroy.push_back(e.id);
					}
				}
			});

			if (!to_destroy.empty()) {
				for (const auto entity_id : to_destroy) {
					inventory::delete_item(entity_id);
				}
			}
		}
	}
}
