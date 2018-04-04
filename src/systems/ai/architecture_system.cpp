#include "stdafx.h"
#include "architecture_system.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../global_assets/architecture_designations.hpp"
#include "../helpers/targeted_flow_map.hpp"
#include <string>

namespace systems {
	namespace architecture_system {

		std::unique_ptr<flow_maps::map_t> architecture_map;

		static bool dirty = true;

		inline static void add_adjacent_same_level(std::vector<std::tuple<int, int>> &starting_points, const int &x, const int &y, const int &z, const int &idx)
		{
			starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x, y - 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x, y + 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y + 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y - 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y + 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y - 1, z), idx));
		}

		inline static void add_adjacent_or_above(std::vector<std::tuple<int, int>> &starting_points, const int &x, const int &y, const int &z, const int &idx)
		{
			starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x, y - 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x, y + 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x, y, z + 1), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y + 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y - 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y + 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y - 1, z), idx));
		}

		inline static void add_adjacent_or_below(std::vector<std::tuple<int, int>> &starting_points, const int &x, const int &y, const int &z, const int &idx)
		{
			starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x, y - 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x, y + 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x, y, z - 1), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y + 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y - 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y + 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y - 1, z), idx));

		}

		inline static void add_adjacent_all(std::vector<std::tuple<int, int>> &starting_points, const int &x, const int &y, const int &z, const int &idx)
		{
			starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x, y - 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x, y + 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x, y, z - 1), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x, y, z + 1), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y + 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x + 1, y - 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y + 1, z), idx));
			starting_points.emplace_back(std::make_tuple(mapidx(x - 1, y - 1, z), idx));

		}

		static void make_architecture_map()
		{
			// This builds a map of available architecure orders, using what is basically a Dijkstra map (flow map).
			std::vector<std::tuple<int, int>> starting_points;
			for (const auto &operation : architecture_designations->architecture)
			{
				const auto idx = operation.first;
				const auto type = operation.second;
				int x,y,z;
				std::tie(x,y,z) = idxmap(idx);

				switch (type)
				{
				case 0: add_adjacent_same_level(starting_points, x, y, z, idx); break; // WALL
				case 1: add_adjacent_same_level(starting_points, x, y, z, idx); break; // FLOOR
				case 2: add_adjacent_or_above(starting_points, x, y, z, idx); break; // UP
				case 3: add_adjacent_or_below(starting_points, x, y, z, idx); break; // DOWN
				case 4: add_adjacent_all(starting_points, x, y, z, idx); break; // UP/DOWN
				case 5: add_adjacent_or_above(starting_points, x, y, z, idx); break; // RAMP
				case 6: add_adjacent_same_level(starting_points, x, y, z, idx); break;  // BRIDGE
				default: throw std::runtime_error("Unknown architecture type: " + std::to_string(type));
				}
			}

			architecture_map->fill_map(starting_points);
		}

		void run(const double &duration_ms)
		{
			if (!architecture_map) architecture_map = std::make_unique<flow_maps::map_t>();
			if (dirty) {
				make_architecture_map();
			}
			dirty = false;
		}

		void architecture_map_changed()
		{
			make_architecture_map();
			dirty = false;
		}
	}
}
