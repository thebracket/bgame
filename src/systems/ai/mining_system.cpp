#include "stdafx.h"
#include "mining_system.hpp"
#include "ai_status_effects.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/game_mining.hpp"

namespace systems {
	namespace mining_system {

		using namespace bengine;
		using namespace region;

		std::array<mine_map_entry_t, REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH> mining_map;
		bool dirty = true;

		static void walk_mining_map(const int x, const int y, const int z, const int target, const int distance) {
			if (distance > 253) return; // bail out because we've gone too far
			if (x < 1 || x > REGION_WIDTH - 1 || y < 1 || y > REGION_HEIGHT - 1 || z < 1 || z > REGION_DEPTH - 1) return; // Bounds check
			const auto idx = mapidx(x, y, z);
			if (!flag(idx, CAN_STAND_HERE)) return; // bail out, because you can't go here
			if (mining_map[idx].distance > distance) { // Ignore any cases in which the tile already has a better option
				mining_map[idx].distance = distance;
				mining_map[idx].target = target;

				if (flag(idx, CAN_GO_NORTH)) walk_mining_map(x, y - 1, z, target, distance + 1);
				if (flag(idx, CAN_GO_SOUTH)) walk_mining_map(x, y + 1, z, target, distance + 1);
				if (flag(idx, CAN_GO_EAST)) walk_mining_map(x + 1, y, z, target, distance + 1);
				if (flag(idx, CAN_GO_WEST)) walk_mining_map(x - 1, y - 1, z, target, distance + 1);
				if (flag(idx, CAN_GO_UP)) walk_mining_map(x, y, z + 1, target, distance + 1);
				if (flag(idx, CAN_GO_DOWN)) walk_mining_map(x, y, z - 1, target, distance + 1);
			}
		}

		static void make_mining_map() {
			// This builds a map of available mining opportunities, using what is basically a Dijkstra map (flow map).
			auto filler = mine_map_entry_t{ std::numeric_limits<uint8_t>::max(), -1 };
			std::fill(mining_map.begin(), mining_map.end(), filler);

			std::vector<std::tuple<int, int, int, int>> starting_points; // x,y,z,operation
			for (const auto &operation : mining_designations->mining_targets) {
				const int idx = operation.first;
				const uint8_t type = operation.second;
				auto[x, y, z] = idxmap(idx);

				if (type == MINE_DIG) {
					// Digging can happen on any adjacent tile.
					starting_points.emplace_back( std::make_tuple(x - 1, y, z, idx ));
					starting_points.emplace_back( std::make_tuple(x + 1, y, z, idx ));
					starting_points.emplace_back( std::make_tuple(x, y - 1, z, idx ));
					starting_points.emplace_back( std::make_tuple(x, y + 1, z, idx ));
				}
				else if (type == MINE_CHANNEL) {
					// Channeling - can stand on the square, the square below, or a spot neighboring the top
					starting_points.emplace_back(std::make_tuple(x, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y, z-1, idx));
					starting_points.emplace_back(std::make_tuple(x - 1, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x + 1, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y - 1, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y + 1, z, idx));
				}
				else if (type == MINE_RAMP) {
					// Ramps - can stand on the square, the square above, or a spot neighboring the bottom
					starting_points.emplace_back(std::make_tuple(x, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y, z + 1, idx));
					starting_points.emplace_back(std::make_tuple(x - 1, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x + 1, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y - 1, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y + 1, z, idx));
				}
				else if (type == MINE_STAIRS_UP) {
					// Up stairs - stand on square or square above
					starting_points.emplace_back(std::make_tuple(x, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y, z + 1, idx));
					starting_points.emplace_back(std::make_tuple(x - 1, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x + 1, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y - 1, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y + 1, z, idx));
				}
				else if (type == MINE_STAIRS_DOWN) {
					// Up stairs - stand on square or square below
					starting_points.emplace_back(std::make_tuple(x, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y, z - 1, idx));
					starting_points.emplace_back(std::make_tuple(x - 1, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x + 1, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y - 1, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y + 1, z, idx));
				}
				else if (type == MINE_STAIRS_UPDOWN) {
					starting_points.emplace_back(std::make_tuple(x, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y, z - 1, idx));
					starting_points.emplace_back(std::make_tuple(x, y, z + 1, idx));
					starting_points.emplace_back(std::make_tuple(x - 1, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x + 1, y, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y - 1, z, idx));
					starting_points.emplace_back(std::make_tuple(x, y + 1, z, idx));
				}
			}

			for (const auto &loc : starting_points) {
				walk_mining_map(std::get<0>(loc), std::get<1>(loc), std::get<2>(loc), std::get<3>(loc), 0);
			}
		}

		void run(const double &duration_ms) {
			if (dirty) {
				make_mining_map();
			}
			dirty = false;
		}

		void mining_map_changed() {
			make_mining_map();
			dirty = false;
		}
	}
}