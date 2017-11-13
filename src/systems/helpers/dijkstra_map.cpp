#include "dijkstra_map.hpp"
#include "../../planet/region/region.hpp"

namespace systems {
	namespace dijkstra {

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

					auto [x,y,z] = idxmap(open_node.first);

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

					auto &[x,y,z] = idxmap(open_node.first);

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
			auto &[dx,dy,dz] = idxmap(candidates.begin()->second);
			return position_t{ dx, dy, dz };
		}

	}
}