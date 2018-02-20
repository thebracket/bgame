#pragma once

#include <array>
#include <memory>
#include "../../planet/region/region.hpp"

namespace flow_maps
{
	class map_t;
}

namespace systems {
	namespace architecture_system {		
		extern std::unique_ptr<flow_maps::map_t> architecture_map;

		void run(const double &duration_ms);
		void architecture_map_changed();
	}
}