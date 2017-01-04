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
#include "../distance_map_system.hpp"
#include "settler_move_to.hpp"
#include "../../messages/entity_moved_message.hpp"

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

    const int idx = mapidx(pos);
	if (ai.job_type_minor == JM_FIND_BED) {
        const int16_t distance = bed_map.distance_map[idx];
        if (distance > MAX_DIJSTRA_DISTANCE) {
            // There is no bed available - sleep rough!
            // TODO: Bad thoughts!
            emit_deferred(log_message{LOG{}.settler_name(entity.id)->text(" cannot find a bed, and is sleeping rough.")->chars});
            ai.job_type_minor = JM_SLEEP;
            ai.job_status = "Sleeping";
            ai.target_id = 0;
            return;
        } else {
            ai.job_type_minor = JM_GO_TO_BED;
            change_job_status(ai, name, "Going to bed");
            return;
        }
	}

	if (ai.job_type_minor == JM_GO_TO_BED) {
        const int16_t distance = bed_map.distance_map[idx];
        if (distance == 0) {
            std::size_t bed_id = 0;
            each<construct_provides_sleep_t, position_t>([&bed_id, &pos] (entity_t &e, construct_provides_sleep_t &sleep, position_t &bpos) {
                if (pos == bpos) bed_id = e.id;
            });
            if (bed_id > 0) {
                auto bed = rltk::entity(bed_id);
                if (bed) {
                    bed->component<construct_provides_sleep_t>()->claimed = true;
                    ai.target_id = bed_id;
                    emit(bed_changed_message{});
                }
            }
            ai.job_type_minor = JM_SLEEP;
            ai.job_status = "Sleeping";
            return;
        } else if (distance > MAX_DIJSTRA_DISTANCE) {
            // There is no bed available - sleep rough!
            // TODO: Bad thoughts!
            emit_deferred(log_message{LOG{}.settler_name(entity.id)->text(" cannot find a bed, and is sleeping rough.")->chars});
            ai.job_type_minor = JM_SLEEP;
            ai.job_status = "Sleeping";
            ai.target_id = 0;
            return;
        } else {
            position_t destination = bed_map.find_destination(pos);
            move_to(entity, pos, destination);
            return;
        }
	}

	if (ai.job_type_minor == JM_SLEEP) {

		auto health = entity.component<health_t>();
		if (health && health->current_hitpoints < health->max_hitpoints && rng.roll_dice(1,20) + stat_modifier(stats.constitution) > 12) {
			++health->current_hitpoints;
		}

		emit_deferred(renderables_changed_message{});
	}
}