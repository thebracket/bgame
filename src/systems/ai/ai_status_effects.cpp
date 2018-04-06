#include "stdafx.h"
#include "ai_status_effects.hpp"

namespace systems {
	namespace ai_status_effects {
		void run(const double &duration_ms) {
			using namespace bengine;

			each<health_t, ai_tag_my_turn_t>([](entity_t &e, health_t &health, ai_tag_my_turn_t &t) {
				// If unconscious, cancel the turn
				if (health.unconscious) {
					delete_component<ai_tag_my_turn_t>(e.id);
				}

				// If stunned, cancel the turn and decrement the counter
				if (health.stunned_counter > 0)
				{
					--health.stunned_counter;
					delete_component<ai_tag_my_turn_t>(e.id);
				}
			});
		}
	}
}
