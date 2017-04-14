#include "lever_work.hpp"
#include "../settler_work_time.hpp"
#include "../../../messages/messages.hpp"
#include "../../../main/game_globals.hpp"
#include "../../ai/path_finding.hpp"
#include "../../ai/mining_system.hpp"
#include "../../ai/inventory_system.hpp"
#include "../../ai/workflow_system.hpp"
#include "../../ai/wildlife_population_system.hpp"
#include "../../damage/weapons_helpers.hpp"
#include "../../components/item_carried.hpp"
#include "../idle_mode.hpp"
#include "../settler_glyph.hpp"
#include "../settler_job_status.hpp"
#include "../settler_drop_tool.hpp"
#include "../settler_cancel_action.hpp"
#include "../pathfinding.hpp"
#include "../initiative.hpp"
#include "../../../messages/log_message.hpp"
#include "../../../components/logger.hpp"
#include "../../../components/health.hpp"
#include "../../../components/renderable.hpp"
#include "../../../components/corpse_harvestable.hpp"
#include "../../../components/construct_provides_sleep.hpp"
#include "../../../components/viewshed.hpp"
#include "../../../components/smoke_emitter.hpp"
#include "../../../components/grazer_ai.hpp"
#include "../../../components/sentient_ai.hpp"
#include "../../../components/lightsource.hpp"
#include "../../../components/falling.hpp"
#include "../../../components/item.hpp"
#include "../world_queries.hpp"
#include "../settler_wander.hpp"
#include "../settler_move_to.hpp"
#include "../../../raws/plants.hpp"
#include "../../ai/distance_map_system.hpp"
#include "../settler_move_to.hpp"
#include "../../../components/bridge.hpp"
#include "../../../components/receives_signal.hpp"
#include "../../../components/lever.hpp"

using namespace rltk;
using tasks::become_idle;
using tasks::change_settler_glyph;
using tasks::change_job_status;
using tasks::drop_current_tool;
using tasks::cancel_action;
using tasks::follow_path;
using tasks::follow_result_t;
using tasks::calculate_initiative;

void do_lever_pull(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name)
{
    const int idx = mapidx(pos);

    if (ai.job_type_minor == JM_GO_TO_LEVER) {
        const auto distance = levers_map.get(idx);
        if (distance >= MAX_DIJSTRA_DISTANCE) {
            cancel_action(e, ai, stats, species, pos, name, "No route to lever");
            return;
        }
        if (distance == 0) {
            ai.job_type_minor = JM_PULLIT;
            ai.job_status = "Pulling lever";
            return;
        }
        position_t destination = levers_map.find_destination(pos);
        move_to(e, pos, destination);
        return;
    }

    if (ai.job_type_minor == JM_PULLIT) {
        std::size_t lever_id = 0;
        each<lever_t, position_t>([&lever_id, &pos] (entity_t &E, lever_t &l, position_t &p) {
            if (p == pos) lever_id = E.id;
        });
        if (lever_id == 0) throw std::runtime_error("Unidentified lever!");

        // Remove from designations
        designations->levers_to_pull.erase(std::remove_if(
                    designations->levers_to_pull.begin(),
                    designations->levers_to_pull.end(),
                    [&lever_id] (auto n) { return n == lever_id; }
                ), designations->levers_to_pull.end());

        // Update the lever map
        emit_deferred(leverpull_changed_message{});

        // Actually pull the lever
        emit_deferred(lever_pulled_message{lever_id});

        // Idle
        become_idle(e, ai, name);
        return;
    }
}