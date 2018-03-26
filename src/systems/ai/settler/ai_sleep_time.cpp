#include "stdafx.h"
#include "ai_sleep_time.hpp"
#include "../distance_map_system.hpp"
#include "../../gui/log_system.hpp"
#include "../../physics/movement_system.hpp"
#include "../../../global_assets/game_pause.hpp"
#include "../../../planet/indices.hpp"
#include "../../helpers/targeted_flow_map.hpp"

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
					const auto bed_target = distance_map::bed_map->find_nearest_reachable_target(pos);
					if (bed_target.target == 0 || !bed_target.path->success)
					{
						// We couldn't find a bed
						sleep.is_sleeping = true;
						// TODO: Bad thoughts!
						logging::log_message msg{ LOG{}.settler_name(e.id)->text(" cannot find a bed, and is sleeping rough.")->chars };
						logging::log(msg);
						return;
					} else if (bed_target.path->destination == pos)
					{
						// We've reached the bed
						sleep.is_sleeping = true;

						// Find the bed and claim it
						each<construct_provides_sleep_t, position_t>([&e, &pos](entity_t &BED, construct_provides_sleep_t &SLEEP, position_t &bpos) {
							if (pos == bpos) {
								BED.assign(claimed_t{ e.id });
								distance_map::refresh_bed_map();
							}
						});
						return;
					} else
					{
						// We want to path towards it
						const auto next_step = bed_target.path->steps.front();
						bed_target.path->steps.pop_front();
						if (next_step.x > 0 && next_step.x < REGION_WIDTH && next_step.y > 0 &&
							next_step.y < REGION_HEIGHT && next_step.z > 0 && next_step.z < REGION_DEPTH
							&& region::flag(mapidx(next_step), tile_flags::CAN_STAND_HERE))
						{
							movement::move_to(e, pos, next_step);
						}
					}
				});
			}
		}
	}
}
