#pragma once

#include <memory>

namespace flow_maps
{
	class map_t; // Forward
}

namespace systems {
	namespace mining_system {
		extern std::unique_ptr<flow_maps::map_t> mining_map;

		void run(const double &duration_ms);
		void mining_map_changed();
	}
}