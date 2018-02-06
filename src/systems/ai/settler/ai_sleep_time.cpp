#include "../distance_map_system.hpp"
#include "../../gui/log_system.hpp"
#include "../../../components/logger.hpp"
#include "../../physics/movement_system.hpp"
#include "ai_sleep_time.hpp"
#include "../../../global_assets/game_pause.hpp"
#include "../../../components/sleep_clock_t.hpp"
#include "../../../components/settler_ai.hpp"
#include "../../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../../components/construct_provides_sleep.hpp"
#include "../../../components/claimed_t.hpp"
#include "../../../components/ai_tags/ai_tag_sleep_shift.hpp"

namespace systems {
	namespace ai_sleep_time {

		using namespace bengine;

		void run(const double &duration_ms) {
			if (hour_elapsed) {
				each<sleep_clock_t>([](entity_t &e, sleep_clock_t &sleep) {
					if (sleep.is_sleeping) {
						if (sleep.sleep_requirement > 0) {
							--sleep.sleep_requirement;
						}
					}
					else {
						++sleep.sleep_requirement;
					}
				});

				each<settler_ai_t, ai_tag_my_turn_t, sleep_clock_t, position_t>([]
				(entity_t &e, settler_ai_t &ai, ai_tag_my_turn_t &turn, sleep_clock_t &sleep, position_t &pos)
				{
					auto schedule = e.component<ai_tag_sleep_shift_t>();
					if (schedule == nullptr) {
						sleep.is_sleeping = false;
						each<construct_provides_sleep_t, claimed_t>([&e](entity_t &E, construct_provides_sleep_t &s, claimed_t &c) {
							if (c.claimed_by == e.id) {
								delete_component<claimed_t>(E.id);
							}
						});
						return;
					}

					// It's my turn, and I'm sleeping
					delete_component<ai_tag_my_turn_t>(e.id);

					// Are we in bed already?
					if (sleep.is_sleeping) return;

					// Find a bed
					const int idx = mapidx(pos);
					const int16_t distance = distance_map::bed_map.get(idx);
					if (distance == 0) {
						// We've reached a bed - sleepy time!
						sleep.is_sleeping = true;

						// Find the bed and claim it
						each<construct_provides_sleep_t, position_t>([&e, &pos](entity_t &BED, construct_provides_sleep_t &sleep, position_t &bpos) {
							if (pos == bpos) {
								BED.assign(claimed_t{ e.id });
								distance_map::refresh_bed_map();
							}
						});
						return;
					}
					else if (distance >= dijkstra::MAX_DIJSTRA_DISTANCE) {
						// There is no bed - sleep rough
						sleep.is_sleeping = true;
						// TODO: Bad thoughts!
						logging::log_message msg{ LOG{}.settler_name(e.id)->text(" cannot find a bed, and is sleeping rough.")->chars };
						logging::log(msg);
						return;
					}
					else {
						// Path towards the bed
						position_t destination = distance_map::bed_map.find_destination(pos);
						movement::move_to(e, pos, destination);
						return;
					}
				});
			}
		}
	}
}