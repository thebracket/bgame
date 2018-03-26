#include "stdafx.h"
#include "ai_status_effects.hpp"

namespace systems {
	namespace ai_status_effects {
		void run(const double &duration_ms) {
			using namespace bengine;

			each<health_t, ai_tag_my_turn_t>([](entity_t &e, health_t &health, ai_tag_my_turn_t &t) {
				// If unsconscious, cancel the turn
				if (health.unconscious) {
					//std::cout << "Cancelling turn - unconcscious\n";
					delete_component<ai_tag_my_turn_t>(e.id);
				}
			});
		}
	}
}
