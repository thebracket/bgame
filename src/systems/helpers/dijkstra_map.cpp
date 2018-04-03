#include "stdafx.h"
#include "dijkstra_map.hpp"
#include "../../planet/region/region.hpp"
#include <stack>
#include <map>

using namespace tile_flags;

namespace systems {
	namespace dijkstra {

		dijkstra_map::dijkstra_map() {
			distance_map_.resize(REGION_TILES_COUNT);
			std::fill(distance_map_.begin(), distance_map_.end(), MAX_DIJSTRA_DISTANCE);
		}

		int16_t dijkstra_map::get(const std::size_t &idx) {
			return distance_map_[idx];
		}

		inline void dm_add_candidate(std::stack<std::pair<int, int>> &open_nodes, const int &x, const int &y, const int &z, const int &distance) {
			using namespace region;
			const auto idx = mapidx(x, y, z);
			//if (water_level(idx) == 0) {
				open_nodes.push(std::make_pair(idx, distance));
			//}
		}

		void dijkstra_map::update(const std::vector<int> &starting_points) {
			using namespace region;
			std::fill(distance_map_.begin(), distance_map_.end(), MAX_DIJSTRA_DISTANCE);

			// Populate the open list with starting points
			std::stack<std::pair<int, int>> open_nodes;
			for (const auto &sp : starting_points) {
				open_nodes.push(std::make_pair(sp, 0));
			}

			// Iterate open nodes list
			while (!open_nodes.empty()) {
				const auto open_node = open_nodes.top();
				open_nodes.pop();

				if (distance_map_[open_node.first] > open_node.second && open_node.second < MAX_DIJSTRA_DISTANCE)
				{
					distance_map_[open_node.first] = open_node.second;

					auto [x,y,z] = idxmap(open_node.first);

					const auto flags = get_flag_reference(open_node.first);

					if (x < REGION_WIDTH - 1 && flags.test(CAN_GO_EAST)) {
						dm_add_candidate(open_nodes, x + 1, y, z, open_node.second + 1);
					}
					if (x > 0 && flags.test(CAN_GO_WEST)) {
						dm_add_candidate(open_nodes, x - 1, y, z, open_node.second + 1);
					}
					if (y < REGION_HEIGHT - 1 && flags.test(CAN_GO_SOUTH)) {
						dm_add_candidate(open_nodes, x, y + 1, z, open_node.second + 1);
					}
					if (y > 0 && flags.test(CAN_GO_NORTH)) {
						dm_add_candidate(open_nodes, x, y - 1, z, open_node.second + 1);
					}
					if (z > 0 && flags.test(CAN_GO_DOWN)) {
						dm_add_candidate(open_nodes, x, y, z - 1, open_node.second + 1);
					}
					if (z < REGION_DEPTH - 1 && flags.test(CAN_GO_UP)) {
						dm_add_candidate(open_nodes, x, y, z + 1, open_node.second + 1);
					}
					/*if (x < REGION_WIDTH - 1 && y > 0 && flags.test(CAN_GO_NORTH_EAST)) {
						dm_add_candidate(open_nodes, x + 1, y - 1, z, open_node.second + 1);
					}
					if (x < REGION_WIDTH - 1 && y < REGION_HEIGHT-1 && flags.test(CAN_GO_SOUTH_EAST)) {
						dm_add_candidate(open_nodes, x + 1, y + 1, z, open_node.second + 1);
					}
					if (x > 0 && y > 0 && flags.test(CAN_GO_NORTH_WEST)) {
						dm_add_candidate(open_nodes, x - 1, y - 1, z, open_node.second + 1);
					}
					if (x > 0 && y < REGION_HEIGHT-1 && flags.test(CAN_GO_SOUTH_WEST)) {
						dm_add_candidate(open_nodes, x - 1, y + 1, z, open_node.second + 1);
					}*/
				}
			}
		}

		
		position_t dijkstra_map::find_destination(const position_t &pos) {
			using namespace region;
		
			const auto idx = mapidx(pos);
			std::map<int16_t, int> candidates;
			if (pos.x > 0 && flag(idx, CAN_GO_WEST)) {
				const auto destidx = mapidx(pos.x - 1, pos.y, pos.z);
				candidates.insert(std::make_pair(distance_map_[destidx], destidx));
			}
			if (pos.x < REGION_WIDTH - 1 && flag(idx, CAN_GO_EAST)) {
				const auto destidx = mapidx(pos.x + 1, pos.y, pos.z);
				candidates.insert(std::make_pair(distance_map_[destidx], destidx));
			}
			if (pos.y > 0 && flag(idx, CAN_GO_NORTH)) {
				const auto destidx = mapidx(pos.x, pos.y - 1, pos.z);
				candidates.insert(std::make_pair(distance_map_[destidx], destidx));
			}
			if (pos.y < REGION_HEIGHT - 1 && flag(idx, CAN_GO_SOUTH)) {
				const auto destidx = mapidx(pos.x, pos.y + 1, pos.z);
				candidates.insert(std::make_pair(distance_map_[destidx], destidx));
			}
			if (pos.z > 0 && flag(idx, CAN_GO_DOWN)) {
				const auto destidx = mapidx(pos.x, pos.y, pos.z - 1);
				candidates.insert(std::make_pair(distance_map_[destidx], destidx));
			}
			if (pos.z < REGION_DEPTH - 1 && flag(idx, CAN_GO_UP)) {
				const auto destidx = mapidx(pos.x, pos.y, pos.z + 1);
				candidates.insert(std::make_pair(distance_map_[destidx], destidx));
			}
			if (pos.x > 0 && pos.y > 0 && flag(idx, CAN_GO_NORTH_WEST))
			{
				const auto destidx = mapidx(pos.x - 1, pos.y - 1, pos.z);
				candidates.insert(std::make_pair(distance_map_[destidx], destidx));
			}
			if (pos.x < REGION_WIDTH-1 && pos.y > 0 && flag(idx, CAN_GO_NORTH_EAST))
			{
				const auto destidx = mapidx(pos.x + 1, pos.y - 1, pos.z);
				candidates.insert(std::make_pair(distance_map_[destidx], destidx));
			}
			if (pos.x > 0 && pos.y < REGION_HEIGHT-1 && flag(idx, CAN_GO_SOUTH_WEST))
			{
				const auto destidx = mapidx(pos.x - 1, pos.y + 1, pos.z);
				candidates.insert(std::make_pair(distance_map_[destidx], destidx));
			}
			if (pos.x < REGION_WIDTH-1 && pos.y < REGION_HEIGHT - 1 && flag(idx, CAN_GO_SOUTH_EAST))
			{
				const auto destidx = mapidx(pos.x + 1, pos.y + 1, pos.z);
				candidates.insert(std::make_pair(distance_map_[destidx], destidx));
			}
			const auto &[dx,dy,dz] = idxmap(candidates.begin()->second);
			return position_t{ dx, dy, dz };
		}

	}
}
