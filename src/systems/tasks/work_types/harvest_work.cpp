#include "harvest_work.hpp"
#include "../settler_work_time.hpp"
#include "../../../messages/messages.hpp"
#include "../../../main/game_globals.hpp"
#include "../../path_finding.hpp"
#include "../../mining_system.hpp"
#include "../../inventory_system.hpp"
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
#include "../world_queries.hpp"
#include "../settler_sleep.hpp"
#include "../settler_wander.hpp"
#include "../settler_move_to.hpp"
#include "../../../raws/plants.hpp"

using namespace rltk;
using tasks::become_idle;
using tasks::change_settler_glyph;
using tasks::change_job_status;
using tasks::drop_current_tool;
using tasks::cancel_action;
using tasks::follow_path;
using tasks::follow_result_t;
using tasks::calculate_initiative;

void do_harvesting(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name)
{
    if (ai.job_type_minor == JM_FIND_HARVEST) {
        position_t harvest_pos = position_t{ai.target_x, ai.target_y, ai.target_z};
        ai.current_path = find_path(pos, harvest_pos);
        if (!ai.current_path->success) {
            cancel_action(e, ai, stats, species, pos, name, "No route to harvest");
            return;
        }
        ai.job_type_minor = JM_GO_TO_HARVEST;
        change_job_status(ai, name, "Traveling to harvest.");
        return;
    }

    if (ai.job_type_minor == JM_GO_TO_HARVEST) {
        tasks::try_path(e, ai, pos,
                        [] () {}, // Do nothing on success
                        [&ai, &name] () {
                            ai.current_path.reset();
                            ai.job_type_minor = JM_DO_HARVEST;
                            change_job_status(ai, name, "Harvesting");
                        }, // On arrival
                        [&e, &ai, &stats, &species, &pos, &name] () {
                            cancel_action(e, ai, stats, species, pos, name, "No route to harvest");
                        }
        );
        return;
    }

    if (ai.job_type_minor == JM_DO_HARVEST) {
        // Remove the harvest designation
        const int idx = mapidx(ai.target_x, ai.target_y, ai.target_z);
        designations->harvest.erase(std::remove_if(
                designations->harvest.begin(),
                designations->harvest.end(),
                [&idx] (std::pair<bool,position_t> p) { return idx == mapidx(p.second); }
                                    ),
                                    designations->harvest.end());

        // Create the harvesting result
        if (current_region->tile_vegetation_type[idx] == 0) {
            cancel_action(e, ai, stats, species, pos, name, "Nothing to harvest");
            return;
        }
        const plant_t plant = get_plant_def(current_region->tile_vegetation_type[idx]);
        const std::string result = plant.provides[current_region->tile_vegetation_lifecycle[idx]];
        if (result != "none") {
            std::string mat_type = "organic";
            auto item_finder = item_defs.find(result);
            if (item_finder != item_defs.end()) {
                if (item_finder->second.categories.test(ITEM_FOOD)) mat_type="food";
                if (item_finder->second.categories.test(ITEM_SPICE)) mat_type="spice";
            }
            auto item = spawn_item_on_ground_ret(ai.target_x, ai.target_y, ai.target_z, result, get_material_by_tag(mat_type).get());
            item->component<item_t>()->item_name = plant.name;
        }

        // Knock tile back to germination
        current_region->tile_vegetation_lifecycle[idx] = 0;
        current_region->tile_vegetation_ticker[idx] = 0;

        // Become idle - done
        become_idle(e, ai, name);
        return;
    }
}
