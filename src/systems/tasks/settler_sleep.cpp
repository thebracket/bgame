#include "settler_sleep.hpp"
#include "settler_job_status.hpp"
#include "../../components/logger.hpp"
#include "../../components/renderable.hpp"
#include "../../components/construct_provides_sleep.hpp"
#include "../../components/health.hpp"
#include "settler_drop_tool.hpp"
#include "settler_cancel_action.hpp"
#include "../path_finding.hpp"
#include "settler_glyph.hpp"
#include "../messages/messages.hpp"
#include "pathfinding.hpp"

using namespace rltk;
using tasks::change_settler_glyph;
using tasks::change_job_status;
using tasks::drop_current_tool;
using tasks::cancel_action;
using tasks::follow_path;
using tasks::follow_result_t;

void do_sleep_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.current_tool != 0) drop_current_tool(entity, ai, pos);

	if (ai.job_type_major != JOB_IDLE && ai.job_type_major != JOB_SLEEP) {
		cancel_action(entity, ai, stats, species, pos, name, "Bed time!");
		return;
	}
	if (ai.job_type_major == JOB_IDLE) {
		ai.job_type_major = JOB_SLEEP;
		ai.job_type_minor = JM_FIND_BED;
		change_job_status(ai, name, "Looking for a bed");
		return;
	}
	if (ai.job_type_major != JOB_SLEEP) throw std::runtime_error("Sleep mode, but shouldn't have made it to here.\n");

	if (ai.job_type_minor == JM_FIND_BED) {
		// Find nearest unclaimed bed
		std::map<float, std::tuple<construct_provides_sleep_t,position_t,std::size_t>> bed_candidates;
		each<construct_provides_sleep_t, position_t>([&bed_candidates, &pos] (entity_t &e, construct_provides_sleep_t &bed, position_t &bed_pos) {
			if (!bed.claimed) {
				if (find_path(pos, bed_pos)->success) {
					bed_candidates[distance3d(pos.x, pos.y, pos.z, bed_pos.x, bed_pos.y, bed_pos.z)] = std::make_tuple(bed, bed_pos, e.id);
				}
			}
		});
		if (bed_candidates.empty()) {
			emit_deferred(log_message{LOG{}.settler_name(entity.id)->text(" cannot find a bed, and is sleeping rough.")->chars});
			ai.job_type_minor = JM_SLEEP;
			ai.target_id = 0;
			return;
		}

		// Claim it
		ai.target_x = std::get<1>(bed_candidates.begin()->second).x;
		ai.target_y = std::get<1>(bed_candidates.begin()->second).y;
		ai.target_z = std::get<1>(bed_candidates.begin()->second).z;
		ai.target_id = std::get<2>(bed_candidates.begin()->second);
		rltk::entity(ai.target_id)->component<construct_provides_sleep_t>()->claimed = true;

		// Set the path
		ai.current_path = find_path(pos, position_t{ai.target_x, ai.target_y, ai.target_z});
		if (!ai.current_path->success) {
			std::cout << "Warning: no path to bed found.\n";
			ai.current_path.reset();
			rltk::entity(ai.target_id)->component<construct_provides_sleep_t>()->claimed = false;
			return;
		}
		ai.job_type_minor = JM_GO_TO_BED;
		change_job_status(ai, name, "Going to bed");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_BED) {
		tasks::try_path(entity, ai, pos,
			[] () {}, // Do nothing for success
			[&ai, &name] () {
				ai.job_type_minor = JM_SLEEP;
				change_job_status(ai, name, "Sleeping", true);
			}, // Arrived
			[&ai, &name, &entity] () {
				ai.job_type_minor = JM_FIND_BED;
				change_job_status(ai, name, "Looking for a bed");
				emit_deferred(log_message{LOG{}.settler_name(entity.id)->text(" cannot find a bed, and is sleeping rough.")->chars});

                auto sleep_entity = rltk::entity(ai.target_id);
                if (sleep_entity) {
                    auto sleep_component = sleep_entity->component<construct_provides_sleep_t>();
                    if (sleep_component) {
                        sleep_component->claimed = false;
                        ai.job_type_minor = JM_SLEEP;
                    }
                }
			} // fail
		);
		return;
	}

	if (ai.job_type_minor == JM_SLEEP) {

		auto health = entity.component<health_t>();
		if (health && health->current_hitpoints < health->max_hitpoints && rng.roll_dice(1,20) + stat_modifier(stats.constitution) > 12) {
			++health->current_hitpoints;
		}

		emit_deferred(renderables_changed_message{});
	}
}