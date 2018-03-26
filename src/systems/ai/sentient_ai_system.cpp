#include "stdafx.h"

namespace systems {
	namespace sentient_ai_system {

		using namespace bengine;

		void run(const double &duration_ms) {
			each<sentient_ai>([](entity_t &e, sentient_ai &ai) {
				++ai.days_since_arrival;
			});
		}
	}
}
