#include "architecture_system.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../global_assets/architecture_designations.hpp"
#include <string>

namespace systems {
	namespace architecture_system {

		std::array<architecture_map_entry_t, REGION_DEPTH * REGION_WIDTH * REGION_HEIGHT> architecture_map; // First = distance, second = target cell

		static bool dirty = true;

		static void walk_arch_map(const int x, const int y, const int z, const int target, const int distance) {
			using namespace region;

			if (distance > 253) return; // bail out because we've gone too far
			if (x < 1 || x > REGION_WIDTH - 1 || y < 1 || y > REGION_HEIGHT - 1 || z < 1 || z > REGION_DEPTH - 1) return; // Bounds check
			const auto idx = mapidx(x, y, z);
			if (!flag(idx, CAN_STAND_HERE)) return; // bail out, because you can't go here
			if (architecture_map[idx].distance > distance) { // Ignore any cases in which the tile already has a better option
				architecture_map[idx].distance = distance;
				architecture_map[idx].target = target;

				if (flag(idx, CAN_GO_NORTH)) walk_arch_map(x, y - 1, z, target, distance + 1);
				if (flag(idx, CAN_GO_SOUTH)) walk_arch_map(x, y + 1, z, target, distance + 1);
				if (flag(idx, CAN_GO_EAST)) walk_arch_map(x + 1, y, z, target, distance + 1);
				if (flag(idx, CAN_GO_WEST)) walk_arch_map(x - 1, y, z, target, distance + 1);
				if (flag(idx, CAN_GO_UP)) walk_arch_map(x, y, z + 1, target, distance + 1);
				if (flag(idx, CAN_GO_DOWN)) walk_arch_map(x, y, z - 1, target, distance + 1);
			}
		}

		inline static void add_adjacent_same_level(std::vector<std::tuple<int, int, int, int>> &starting_points, const int &x, const int &y, const int &z, const int &idx)
		{
			starting_points.emplace_back(std::make_tuple(x - 1, y, z, idx));
			starting_points.emplace_back(std::make_tuple(x + 1, y, z, idx));
			starting_points.emplace_back(std::make_tuple(x, y - 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x, y + 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x + 1, y + 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x + 1, y - 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x - 1, y + 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x - 1, y - 1, z, idx));
		}

		inline static void add_adjacent_or_above(std::vector<std::tuple<int, int, int, int>> &starting_points, const int &x, const int &y, const int &z, const int &idx)
		{
			starting_points.emplace_back(std::make_tuple(x - 1, y, z, idx));
			starting_points.emplace_back(std::make_tuple(x + 1, y, z, idx));
			starting_points.emplace_back(std::make_tuple(x, y - 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x, y + 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x, y, z + 1, idx));
			starting_points.emplace_back(std::make_tuple(x + 1, y + 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x + 1, y - 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x - 1, y + 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x - 1, y - 1, z, idx));
		}

		inline static void add_adjacent_or_below(std::vector<std::tuple<int, int, int, int>> &starting_points, const int &x, const int &y, const int &z, const int &idx)
		{
			starting_points.emplace_back(std::make_tuple(x - 1, y, z, idx));
			starting_points.emplace_back(std::make_tuple(x + 1, y, z, idx));
			starting_points.emplace_back(std::make_tuple(x, y - 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x, y + 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x, y, z - 1, idx));
			starting_points.emplace_back(std::make_tuple(x + 1, y + 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x + 1, y - 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x - 1, y + 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x - 1, y - 1, z, idx));

		}

		inline static void add_adjacent_all(std::vector<std::tuple<int, int, int, int>> &starting_points, const int &x, const int &y, const int &z, const int &idx)
		{
			starting_points.emplace_back(std::make_tuple(x - 1, y, z, idx));
			starting_points.emplace_back(std::make_tuple(x + 1, y, z, idx));
			starting_points.emplace_back(std::make_tuple(x, y - 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x, y + 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x, y, z - 1, idx));
			starting_points.emplace_back(std::make_tuple(x, y, z + 1, idx));
			starting_points.emplace_back(std::make_tuple(x + 1, y + 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x + 1, y - 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x - 1, y + 1, z, idx));
			starting_points.emplace_back(std::make_tuple(x - 1, y - 1, z, idx));

		}

		static void make_architecture_map()
		{
			// This builds a map of available architecure orders, using what is basically a Dijkstra map (flow map).
			const auto filler = architecture_map_entry_t{ std::numeric_limits<uint8_t>::max(), -1 };
			std::fill(architecture_map.begin(), architecture_map.end(), filler);

			std::vector<std::tuple<int, int, int, int>> starting_points;
			for (const auto &operation : architecture_designations->architecture)
			{
				const auto idx = operation.first;
				const auto type = operation.second;
				auto[x, y, z] = idxmap(idx);

				switch (type)
				{
				case 0: add_adjacent_same_level(starting_points, x, y, z, idx); break; // WALL
				case 1: add_adjacent_same_level(starting_points, x, y, z, idx); break; // FLOOR
				case 2: add_adjacent_or_above(starting_points, x, y, z, idx); break; // UP
				case 3: add_adjacent_or_below(starting_points, x, y, z, idx); break; // DOWN
				case 4: add_adjacent_all(starting_points, x, y, z, idx); break; // UP/DOWN
				case 5: add_adjacent_or_above(starting_points, x, y, z, idx); break; // RAMP
				case 6: add_adjacent_same_level(starting_points, x, y, z, idx); break;  // BRIDGE
				default: throw std::runtime_error("Unkown architecture type: " + std::to_string(type));
				}
			}

			for (const auto &loc : starting_points) {
				walk_arch_map(std::get<0>(loc), std::get<1>(loc), std::get<2>(loc), std::get<3>(loc), 0);
			}
		}

		void run(const double &duration_ms)
		{
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
