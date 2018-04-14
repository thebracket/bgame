#include "hunger_system.hpp"
#include "components/hunger_t.hpp"
#include "global_assets/game_ecs.hpp"

namespace systems {
	namespace hunger_system {
		void run(const double &duration_ms)
		{
			bengine::each<hunger_t>([] (bengine::entity_t &e, hunger_t &h)
			{
				--h.hunger_clock;
				if (h.hunger_clock < 60) h.is_hungry = true;
				if (h.hunger_clock < -(24 * 60 * 3)) h.is_starving = true;
			});

			bengine::each<thirst_t>([](bengine::entity_t &e, thirst_t &h)
			{
				--h.thirst_clock;
				if (h.thirst_clock < 60) h.is_thirsty = true;
				if (h.thirst_clock < -(24 * 60 * 3)) h.is_dehydrating = true;
			});
		}
	}
}
