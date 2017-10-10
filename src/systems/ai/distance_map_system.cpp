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

		constexpr int16_t MAX_DIJSTRA_DISTANCE = 512;

		struct dijkstra_map {
			dijkstra_map();
			void update(const std::vector<int> starting_points);
			void update_architecture(const std::vector<int> starting_points);
			position_t find_destination(const position_t &pos);
			int16_t get(const std::size_t &idx);

		private:
			std::mutex map_lock;
			std::vector<int16_t> distance_map;
			void update_async(const std::vector<int> &starting_points);
			void update_architecture_async(const std::vector<int> &starting_points);
		};

		dijkstra_map::dijkstra_map() {
			distance_map.resize(REGION_TILES_COUNT);
			std::fill(distance_map.begin(), distance_map.end(), MAX_DIJSTRA_DISTANCE);
		}

		int16_t dijkstra_map::get(const std::size_t &idx) {
			std::lock_guard<std::mutex> lock(map_lock);
			return distance_map[idx];
		}

		inline void dm_add_candidate(std::deque<std::pair<int, int>> &open_nodes, const int &x, const int &y, const int &z, const int &distance) {
			using namespace region;
			const int idx = mapidx(x, y, z);
			if (water_level(idx) < 4) {
				open_nodes.emplace_back(std::make_pair(idx, distance));
			}
		}

		void dijkstra_map::update(const std::vector<int> starting_points) {
			std::thread{ &dijkstra_map::update_async, this, starting_points }.detach();
		}

		void dijkstra_map::update_architecture(const std::vector<int> starting_points) {
			std::thread{ &dijkstra_map::update_architecture_async, this, starting_points }.detach();
		}

		void dijkstra_map::update_async(const std::vector<int> &starting_points)
		{
			using namespace region;
			std::vector<int16_t> new_map;
			new_map.resize(REGION_TILES_COUNT);
			std::fill(new_map.begin(), new_map.end(), MAX_DIJSTRA_DISTANCE);

			// Populate the open list with starting points
			std::deque<std::pair<int, int>> open_nodes;
			for (const int &sp : starting_points) {
				open_nodes.emplace_back(std::make_pair(sp, 0));
			}

			// Iterate open nodes list
			while (!open_nodes.empty()) {
				const std::pair<int, int> open_node = open_nodes.front();
				open_nodes.pop_front();

				if (new_map[open_node.first] > open_node.second && open_node.second < MAX_DIJSTRA_DISTANCE)
				{
					new_map[open_node.first] = open_node.second;

					int x, y, z;
					std::tie(x, y, z) = idxmap(open_node.first);

					if (x < REGION_WIDTH - 1 && flag(open_node.first, CAN_GO_EAST)) {
						dm_add_candidate(open_nodes, x + 1, y, z, open_node.second + 1);
					}
					if (x > 0 && flag(open_node.first, CAN_GO_WEST)) {
						dm_add_candidate(open_nodes, x - 1, y, z, open_node.second + 1);
					}
					if (y < REGION_WIDTH - 1 && flag(open_node.first, CAN_GO_SOUTH)) {
						dm_add_candidate(open_nodes, x, y + 1, z, open_node.second + 1);
					}
					if (y > 0 && flag(open_node.first, CAN_GO_WEST)) {
						dm_add_candidate(open_nodes, x, y - 1, z, open_node.second + 1);
					}
					if (z > 0 && flag(open_node.first, CAN_GO_DOWN)) {
						dm_add_candidate(open_nodes, x, y, z - 1, open_node.second + 1);
					}
					if (z < REGION_DEPTH - 1 && flag(open_node.first, CAN_GO_UP)) {
						dm_add_candidate(open_nodes, x, y, z + 1, open_node.second + 1);
					}
				}
			}

			std::lock_guard<std::mutex> lock(map_lock);
			distance_map = new_map;
		}

		void dijkstra_map::update_architecture_async(const std::vector<int> &starting_points)
		{
			using namespace region;
			std::vector<int16_t> new_map;
			new_map.resize(REGION_TILES_COUNT);
			std::fill(new_map.begin(), new_map.end(), MAX_DIJSTRA_DISTANCE);

			// Populate the open list with starting points
			std::deque<std::pair<int, int>> open_nodes;
			for (const int &sp : starting_points) {
				open_nodes.emplace_back(std::make_pair(sp, 0));
			}

			// Iterate open nodes list
			while (!open_nodes.empty()) {
				const std::pair<int, int> open_node = open_nodes.front();
				open_nodes.pop_front();

				if (new_map[open_node.first] > open_node.second && open_node.second < MAX_DIJSTRA_DISTANCE)
				{
					new_map[open_node.first] = open_node.second;

					int x, y, z;
					std::tie(x, y, z) = idxmap(open_node.first);

					if (x < REGION_WIDTH - 1 && (flag(open_node.first, CAN_GO_EAST) || open_node.second == 0)) {
						dm_add_candidate(open_nodes, x + 1, y, z, open_node.second + 1);
					}
					if (x > 0 && (flag(open_node.first, CAN_GO_WEST) || open_node.second == 0)) {
						dm_add_candidate(open_nodes, x - 1, y, z, open_node.second + 1);
					}
					if (y < REGION_WIDTH - 1 && (flag(open_node.first, CAN_GO_SOUTH) || open_node.second == 0)) {
						dm_add_candidate(open_nodes, x, y + 1, z, open_node.second + 1);
					}
					if (y > 0 && (flag(open_node.first, CAN_GO_WEST) || open_node.second == 0)) {
						dm_add_candidate(open_nodes, x, y - 1, z, open_node.second + 1);
					}
					if (z > 0 && flag(open_node.first, CAN_GO_DOWN)) {
						dm_add_candidate(open_nodes, x, y, z - 1, open_node.second + 1);
					}
					if (z < REGION_DEPTH - 1 && flag(open_node.first, CAN_GO_UP)) {
						dm_add_candidate(open_nodes, x, y, z + 1, open_node.second + 1);
					}
				}
			}

			std::lock_guard<std::mutex> lock(map_lock);
			distance_map = new_map;
		}

		position_t dijkstra_map::find_destination(const position_t &pos) {
			using namespace region;
			std::lock_guard<std::mutex> lock(map_lock);

			const int idx = mapidx(pos);
			std::map<int16_t, int> candidates;
			if (pos.x > 0 && flag(idx, CAN_GO_WEST)) {
				const int destidx = mapidx(pos.x - 1, pos.y, pos.z);
				candidates.insert(std::make_pair(distance_map[destidx], destidx));
			}
			if (pos.x < REGION_WIDTH - 1 && flag(idx, CAN_GO_EAST)) {
				const int destidx = mapidx(pos.x + 1, pos.y, pos.z);
				candidates.insert(std::make_pair(distance_map[destidx], destidx));
			}
			if (pos.y > 0 && flag(idx, CAN_GO_NORTH)) {
				const int destidx = mapidx(pos.x, pos.y - 1, pos.z);
				candidates.insert(std::make_pair(distance_map[destidx], destidx));
			}
			if (pos.y < REGION_HEIGHT - 1 && flag(idx, CAN_GO_SOUTH)) {
				const int destidx = mapidx(pos.x, pos.y + 1, pos.z);
				candidates.insert(std::make_pair(distance_map[destidx], destidx));
			}
			if (pos.z > 0 && flag(idx, CAN_GO_DOWN)) {
				const int destidx = mapidx(pos.x, pos.y, pos.z - 1);
				candidates.insert(std::make_pair(distance_map[destidx], destidx));
			}
			if (pos.z < REGION_DEPTH - 1 && flag(idx, CAN_GO_UP)) {
				const int destidx = mapidx(pos.x, pos.y, pos.z + 1);
				candidates.insert(std::make_pair(distance_map[destidx], destidx));
			}
			int dx, dy, dz;
			std::tie(dx, dy, dz) = idxmap(candidates.begin()->second);
			return position_t{ dx, dy, dz };
		}

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

		using namespace bengine;

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

			each_if<item_t>([](entity_t &e, item_t &i) { return i.claimed == false && i.item_tag == "block"; },
				[&used, &targets](entity_t &e, item_t &i) {
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
				if (item.claimed) return;

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
				if (item.claimed) return;

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
		}
	}
}