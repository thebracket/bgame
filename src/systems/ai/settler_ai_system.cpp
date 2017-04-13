#include "settler_ai_system.hpp"
#include "../../messages/messages.hpp"
#include "../../main/game_globals.hpp"
#include "path_finding.hpp"
#include "mining_system.hpp"
#include "inventory_system.hpp"
#include "workflow_system.hpp"
#include "wildlife_population_system.hpp"
#include "../damage/weapons_helpers.hpp"
#include "../../components/item_carried.hpp"
#include "../tasks/idle_mode.hpp"
#include "../tasks/settler_glyph.hpp"
#include "../tasks/settler_job_status.hpp"
#include "../tasks/settler_drop_tool.hpp"
#include "../tasks/settler_cancel_action.hpp"
#include "../tasks/pathfinding.hpp"
#include "../tasks/initiative.hpp"
#include "../../messages/log_message.hpp"
#include "../../components/logger.hpp"
#include "../../components/health.hpp"
#include "../../components/renderable.hpp"
#include "../../components/corpse_harvestable.hpp"
#include "../../components/construct_provides_sleep.hpp"
#include "../../components/viewshed.hpp"
#include "../../components/smoke_emitter.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/lightsource.hpp"
#include "../../components/falling.hpp"
#include "../../components/initiative.hpp"
#include "../tasks/threat_scanner.hpp"
#include "../tasks/world_queries.hpp"
#include "../tasks/settler_sleep.hpp"
#include "../tasks/settler_wander.hpp"
#include "../tasks/settler_move_to.hpp"
#include "../tasks/settler_work_time.hpp"

#include <iostream>
#include <map>

using namespace rltk;
using tasks::become_idle;
using tasks::change_settler_glyph;
using tasks::change_job_status;
using tasks::drop_current_tool;
using tasks::cancel_action;
using tasks::follow_path;
using tasks::follow_result_t;
using tasks::calculate_initiative;

void settler_ai_system::update(const double duration_ms) {
	each_mbox<action_available_message>([this] (const action_available_message &msg) {

        auto e = entity(msg.entity_id);
        auto ai = e->component<settler_ai_t>();
        if (!ai) return;
        auto stats = e->component<game_stats_t>();
        auto species = e->component<species_t>();
        auto pos = e->component<position_t>();
        auto name = e->component<name_t>();
        auto health = e->component<health_t>();
        auto view = e->component<viewshed_t>();

        if (!stats || !species || !pos || !name || !health || !view) return;

        // Is the settler falling?
        if (e->component<falling_t>()) {
            emit_deferred(LOG{}.settler_name(e->id)->text(" says 'arrrghh'"));
            return;
        }

        if (game_master_mode == ROGUE && e->id == selected_settler) return; // We handle this in the rogue system

        const int shift_id = ai->shift_id;
        const int hour_of_day = calendar->hour;
        const shift_type_t current_schedule = calendar->defined_shifts[shift_id].hours[hour_of_day];

        if (ai->job_type_major == JOB_MINE || ai->job_type_major == JOB_CHOP || ai->job_type_major == JOB_CONST
                   || ai->job_type_major == JOB_REACTION || ai->job_type_major == JOB_BUTCHER || ai->job_type_major == JOB_DECONSTRUCT) {
            // If we have a job to do - keep doing it
            do_work_time(*e, *ai, *stats, *species, *pos, *name);
        } else {
            do_work_time(*e, *ai, *stats, *species, *pos, *name);
        }
	});

    // TODO: Check for living settlers
}

void settler_ai_system::configure() {
	subscribe_mbox<action_available_message>();
	system_name = "Settler AI";
}
