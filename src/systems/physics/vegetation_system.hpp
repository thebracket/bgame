#pragma once

#include <unordered_set>

namespace systems {
	namespace vegetation {
		void run(const double &duration_ms);
		void inflict_damage(const int &idx, const int &dmg);
	}
}