#pragma once

#include <array>
#include "../../planet/region/region.hpp"

namespace systems {
	namespace architecture_system {
		struct architecture_map_entry_t {
			uint8_t distance;
			int target;
		};

		extern std::array<architecture_map_entry_t, REGION_DEPTH * REGION_WIDTH * REGION_HEIGHT> architecture_map; // First = distance, second = target cell

		void run(const double &duration_ms);
		void architecture_map_changed();
	}
}