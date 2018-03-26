#include "stdafx.h"
#include "stockpile_system.hpp"
#include "../../planet/region/region.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/items.hpp"

namespace systems {
	namespace stockpile_system {

		using namespace region;
		using namespace bengine;

		std::unordered_map<int, stockpile_info_t> stockpiles;
		std::unordered_map<int, std::vector<int>> stockpile_targets;
		std::vector<storable_item_t> storable_items;

		using namespace bengine;

		void run(const double &duration_ms) {
			stockpiles.clear();
			stockpile_targets.clear();
			storable_items.clear();

			bool has_stockpiles = false;
			// Build a list of stockpiles and their targets
			each<stockpile_t>([&has_stockpiles](entity_t &e, stockpile_t &sp) {
				has_stockpiles = true;

				stockpile_info_t spi{ e.id, sp.category };
				stockpiles[e.id] = spi;
				for (int i = 0; i<128; ++i) {
					if (sp.category.test(i)) {
						auto finder = stockpile_targets.find(e.id);
						if (finder == stockpile_targets.end()) {
							stockpile_targets[i] = std::vector<int>{ e.id };
						}
						else {
							stockpile_targets[i].push_back(e.id);
						}
					}
				}
			});
			if (!has_stockpiles) return;

			// Build a free tiles list
			for (int i = 0; i<REGION_TILES_COUNT; ++i) {
				const int spid = stockpile_id(i);
				if (spid > 0) {
					++stockpiles[spid].free_capacity;
					stockpiles[spid].open_tiles.insert(i);
				}
			}

			// Find items in stockpiles and items not in stockpiles
			each<item_t, position_t>([](entity_t &e, item_t &i, position_t &pos) {
				const int idx = mapidx(pos);
				if (stockpile_id(idx) > 0) {
					// If it is already in a stockpile, reduce that capacity
					--stockpiles[stockpile_id(idx)].free_capacity;
					stockpiles[stockpile_id(idx)].open_tiles.erase(idx);
					return;
				}

				// If the item is claimed, do nothing
				if (e.component<claimed_t>() != nullptr) return;

				// Determine the destination stockpile; Clothing is a special case
				int item_stockpile_idx = 0;
				if (i.type == CLOTHING) {
					item_stockpile_idx = get_clothing_stockpile();
				}
				else {
					auto finder = get_item_def(i.item_tag);
					if (finder != nullptr) {
						item_stockpile_idx = finder->stockpile_idx;
					}
				}

				// No stockpile for this? Escape!
				if (item_stockpile_idx == 0) return;

				// Look for capacity
				auto finder = stockpile_targets.find(item_stockpile_idx);
				if (finder == stockpile_targets.end()) return; // If there's nowhere interested, escape
				for (const auto &stockpile_id : finder->second) {
					if (stockpiles[stockpile_id].free_capacity > 0) {
						storable_items.emplace_back(storable_item_t{ e.id, stockpile_id });
						return; // Bail out since we've done this item
					}
				}
			});
		}
	}
}
