#pragma once

#include <map>

namespace systems {
	namespace design_architecture {
		void run(const double &duration_ms);

		extern std::map<int, uint8_t> architecture_cursors; // int = mapidx, uint8_t cursor type.
	}
}