#pragma once

#include <set>

namespace systems {
	namespace design_harvest {
		void run(const double &duration_ms);

		extern std::set<int> cursors;
	}
}
