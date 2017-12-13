#include "distance_map_system.hpp"
#include "../../bengine/ecs.hpp"
#include "../../components/position.hpp"
#include "../../global_assets/game_planet.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/corpse_harvestable.hpp"
#include "../../components/construct_provides_sleep.hpp"
#include "../../components/claimed_t.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/item.hpp"
#include "../../components/item_stored.hpp"
#include "../helpers/inventory_assistant.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/items.hpp"
#include <mutex>
#include <vector>

namespace systems {
	namespace distance_map {

		using namespace dijkstra;

		dijkstra_map huntables_map;
		dijkstra_map butcherables_map;
		dijkstra_map bed_map;
		dijkstra_map settler_map;
		dijkstra_map architecure_map;
		dijkstra_map blocks_map;
		dijkstra_map levers_map;
		dijkstra_map axe_map;
		dijkstra_map pick_map;
		dijkstra_map harvest_map;
		bool dijkstra_debug = false;

		bool huntables_dirty = true;
		bool butcherables_dirty = true;
		bool beds_dirty = true;
		bool settlers_dirty = true;
		bool architecture_dirty = true;
		bool blocks_dirty = true;
		bool levers_dirty = true;
		bool axes_dirty = true;
		bool picks_dirty = true;
		bool harvest_dirty = true;

		using namespace bengine;

		void refresh_bed_map() {
			beds_dirty = true;
		}

		void refresh_axe_map() {
			axes_dirty = true;
		}

		void refresh_hunting_map() {
			huntables_dirty = true;
		}

		void refresh_blocks_map() {
			blocks_dirty = true;
		}

		void refresh_architecture_map() {
			architecture_dirty = true;
		}

		void refresh_butcherables_map() {
			butcherables_dirty = true;
		}

		void refresh_pick_map() {
			picks_dirty = true;
		}

		void refresh_harvest_map() {
			harvest_dirty = true;
		}

		void refresh_all_distance_maps() {
			beds_dirty = true;
			axes_dirty = true;
			huntables_dirty = true;
			blocks_dirty = true;
			architecture_dirty = true;
			butcherables_dirty = true;
			picks_dirty = true;
			harvest_dirty = true;
		}

		void update_hunting_map() {
			std::vector<int> huntables;
			each<grazer_ai, position_t>([&huntables](entity_t &e, grazer_ai &ai, position_t &pos) {
				huntables.emplace_back(mapidx(pos));
			});
			huntables_map.update(huntables);
		}

		void update_butcher_map() {
			std::vector<int> butcherables;
			each<corpse_harvestable, position_t>([&butcherables](entity_t &e, corpse_harvestable &corpse, position_t &pos) {
				butcherables.emplace_back(mapidx(pos));
			});
			butcherables_map.update(butcherables);
		}

		void update_bed_map() {
			std::vector<int> beds;
			each<construct_provides_sleep_t, position_t>([&beds](entity_t &e, construct_provides_sleep_t &bed, position_t &pos) {
				auto claimed = e.component<claimed_t>();
				if (claimed == nullptr) {
					beds.emplace_back(mapidx(pos));
				}
			});
			bed_map.update(beds);
		}

		void update_settler_map() {
			std::vector<int> settlers;
			each<settler_ai_t, position_t>([&settlers](entity_t &e, settler_ai_t &settler, position_t &pos) {
				settlers.emplace_back(mapidx(pos));
			});
			settler_map.update(settlers);
		}

		void update_architecure_map() {
			std::vector<int> targets;
			for (auto it = designations->architecture.begin(); it != designations->architecture.end(); ++it) {
				targets.emplace_back(it->first);
			}
			architecure_map.update_architecture(targets);
		}

		void update_blocks_map() {
			std::unordered_set<int> used;
			std::vector<int> targets;

			each_if<item_t>([](entity_t &e, item_t &i) { return i.item_tag == "block"; }, [&used, &targets](entity_t &e, item_t &i) {
				auto is_claimed = e.component<claimed_t>();
				if (is_claimed != nullptr) return;
				auto pos = inventory::get_item_location(e.id);
				if (pos) {
					const int idx = mapidx(*pos);
					if (used.find(idx) == used.end()) {
						used.insert(idx);
						targets.emplace_back(idx);
					}
				}
			}
			);
			blocks_map.update(targets);
		}

		void update_levers_map() {
			std::vector<int> targets;
			for (auto it = designations->levers_to_pull.begin(); it != designations->levers_to_pull.end(); ++it) {
				auto lever_id = *it;
				auto lever_entity = entity(lever_id);
				if (!lever_entity) break;
				auto lever_pos = lever_entity->component<position_t>();
				if (!lever_pos) break;
				targets.emplace_back(mapidx(*lever_pos));
			}
			levers_map.update(targets);
		}

		void update_axe_map() {
			std::vector<int> targets;
			each<item_t>([&targets](entity_t &e, item_t &item) {
				if (!item.category.test(TOOL_CHOPPING)) return; // Not an axe!
				if (e.component<claimed_t>() != nullptr) return; // Don't touch claimed items

				auto pos = e.component<position_t>();
				if (pos != nullptr) {
					targets.emplace_back(mapidx(*pos));
				}
				else {
					auto store = e.component<item_stored_t>();
					if (store != nullptr) {
						auto storage_entity = entity(store->stored_in);
						if (storage_entity) {
							auto spos = storage_entity->component<position_t>();
							targets.emplace_back(mapidx(*spos));
						}
					}
				}
			});
			axe_map.update(targets);
		}

		void update_pick_map() {
			std::vector<int> targets;
			each<item_t>([&targets](entity_t &e, item_t &item) {
				if (!item.category.test(TOOL_DIGGING)) return; // Not an axe!
				if (e.component<claimed_t>() != nullptr) return; // Don't touch claimed items

				auto pos = e.component<position_t>();
				if (pos != nullptr) {
					targets.emplace_back(mapidx(*pos));
				}
				else {
					auto store = e.component<item_stored_t>();
					if (store != nullptr) {
						auto storage_entity = entity(store->stored_in);
						if (storage_entity) {
							auto spos = storage_entity->component<position_t>();
							targets.emplace_back(mapidx(*spos));
						}
					}
				}
			});
			std::cout << "Updating pick map - " << targets.size() << " candidates.\n";
			pick_map.update(targets);
		}

		void update_harvest_map() {
			std::vector<int> targets;
			for (auto it = designations->harvest.begin(); it != designations->harvest.end(); ++it) {
				targets.emplace_back(mapidx(it->second));
			}
			harvest_map.update(targets);
		}

		void run(const double &duration_ms) {
			if (huntables_dirty) {
				update_hunting_map();
				huntables_dirty = false;
			}

			if (butcherables_dirty) {
				update_butcher_map();
				butcherables_dirty = false;
			}

			if (beds_dirty) {
				update_bed_map();
				beds_dirty = false;
			}

			if (settlers_dirty) {
				update_settler_map();
				settlers_dirty = false;
			}

			if (architecture_dirty) {
				update_architecure_map();
				architecture_dirty = false;
			}

			if (blocks_dirty) {
				update_blocks_map();
				blocks_dirty = false;
			}

			if (levers_dirty) {
				update_levers_map();
				levers_dirty = false;
			}

			if (axes_dirty) {
				axes_dirty = false;
				update_axe_map();
			}

			if (picks_dirty) {
				update_pick_map();
				picks_dirty = false;
			}

			if (harvest_dirty) {
				update_harvest_map();
				harvest_dirty = false;
			}
		}
	}
}