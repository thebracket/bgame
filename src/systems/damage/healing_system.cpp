#include "stdafx.h"
#include "healing_system.hpp"

namespace systems {
	namespace healing_system {

		using namespace bengine;

		void run(const double &duration_ms) {
			each<health_t>([](entity_t &e, health_t &h) {
				if (h.max_hitpoints > h.current_hitpoints) {
					++h.current_hitpoints;
					h.unconscious = false;
				}
			});
		}
	}
}
