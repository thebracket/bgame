#pragma once

#include <array>
#include "../../planet/region/region.hpp"

namespace systems {
	namespace mining_system {
		struct mine_map_entry_t {
			uint8_t distance;
			int target;
		};

		extern std::array<mine_map_entry_t, REGION_DEPTH * REGION_WIDTH * REGION_HEIGHT> mining_map; // First = distance, second = target cell

		void run(const double &duration_ms);
		void mining_map_changed();
	}
}