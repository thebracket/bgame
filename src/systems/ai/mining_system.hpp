#pragma once

#include <vector>

namespace systems {
	namespace mining_system {
		extern std::vector<uint8_t> mining_map;
		extern std::vector<int> mining_targets;

		void run(const double &duration_ms);
		void mining_map_changed();
	}
}