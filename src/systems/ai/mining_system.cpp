#include "stdafx.h"
#include "mining_system.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/game_mining.hpp"
#include "../helpers/targeted_flow_map.hpp"

namespace systems {
	namespace mining_system {

		using namespace bengine;
		using namespace region;

		std::unique_ptr<flow_maps::map_t> mining_map;
		static bool dirty = true;

		static void make_mining_map() {
			// This builds a map of available mining opportunities, using what is basically a Dijkstra map (flow map).

			std::vector<std::tuple<int, int>> starting_points; // x,y,z,operation
			for (const auto &operation : mining_designations->mining_targets) {
				const auto idx = operation.first;
				const auto type = operation.second;
				auto[x, y, z] = idxmap(idx);

				if (type == MINE_DIG) {
					// Digging can happen on any adjacent tile.
					starting_points.emplace_back( std::make_tuple(mapidx(x - 1, y, z), idx ));
					starting_points.emplace_back( std::make_tuple(mapidx(x + 1, y, z), idx ));
					starting_points.emplace_back( std::make_tuple(mapidx(x, y - 1, z), idx ));
					starting_points.emplace_back( std::make_tuple(mapidx(x, y + 1, z), idx ));
				}
				else if (type == MINE_CHANNEL) {
					// Channeling - can stand on the square, the square below, or a spot neighboring the top
					//starting_points.emplace_back(std::make_tuple(mapidx(x, y, z), idx)); // We don't really want to stand on the square we are digging down!
					//starting_points.emplace_back(std::make_tuple(mapidx(x, y, z-1), idx)); // Also a bad idea - rocks fall on your head.
					starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y - 1, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y + 1, z), idx));
				}
				else if (type == MINE_RAMP) {
					// Ramps - can stand on the square, the square above, or a spot neighboring the bottom
					starting_points.emplace_back(std::make_tuple(mapidx(x, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y, z + 1), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y - 1, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y + 1, z), idx));
				}
				else if (type == MINE_STAIRS_UP) {
					// Up stairs - stand on square or square above
					starting_points.emplace_back(std::make_tuple(mapidx(x, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y, z + 1), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y - 1, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y + 1, z), idx));
				}
				else if (type == MINE_STAIRS_DOWN) {
					// Up stairs - stand on square or square below
					starting_points.emplace_back(std::make_tuple(mapidx(x, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y, z - 1), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y - 1, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y + 1, z), idx));
				}
				else if (type == MINE_STAIRS_UPDOWN) {
					starting_points.emplace_back(std::make_tuple(mapidx(x, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y, z - 1), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y, z + 1), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y - 1, z), idx));
					starting_points.emplace_back(std::make_tuple(mapidx(x, y + 1, z), idx));
				}
			}

			mining_map->fill_map(starting_points);
		}

		void run(const double &duration_ms) {
			if (!mining_map) mining_map = std::make_unique<flow_maps::map_t>();
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