#pragma once

#include <array>
#include "planet/constants.hpp"

namespace systems {
	namespace gravity {
		void run(const double &duration_ms);
		extern std::array<bool, REGION_TILES_COUNT> supported;
		void tile_was_removed();
	}
}
