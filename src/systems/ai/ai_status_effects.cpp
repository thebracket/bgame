#include "ai_status_effects.hpp"
#include "../../components/health.hpp"
#include "../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../bengine/ecs.hpp"
#include "../../global_assets/game_ecs.hpp"

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
