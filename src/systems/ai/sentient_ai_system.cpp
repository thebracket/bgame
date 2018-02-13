#include "ai_status_effects.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../bengine/ecs.hpp"

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
