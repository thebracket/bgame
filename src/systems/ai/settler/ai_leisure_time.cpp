#include "ai_leisure_time.hpp"
#include "../../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../../components/settler_ai.hpp"
#include "../../../components/ai_tags/ai_tag_leisure_shift.hpp"
#include "../../../components/ai_tags/ai_mode_idle.hpp"
#include "../../physics/movement_system.hpp"
#include "../../../global_assets/game_ecs.hpp"

namespace systems {
	namespace ai_leisure_time {

		using namespace bengine;

		void run(const double &duration_ms) {
			each<settler_ai_t, ai_tag_leisure_shift_t>([](entity_t &e, settler_ai_t &ai, ai_tag_leisure_shift_t &leisure) {
				delete_component<ai_tag_leisure_shift_t>(e.id);
				delete_component<ai_tag_my_turn_t>(e.id);
				e.assign(ai_mode_idle_t{});

				// We don't have any leisure activities yet, so move randomly.
				movement::request_random_move(e.id);
			});
		}
	}
}
