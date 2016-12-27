#include "hauling_work.hpp"
#include "../../inventory_system.hpp"
#include "../../messages/messages.hpp"
#include "../../path_finding.hpp"
#include "../pathfinding.hpp"
#include "../settler_cancel_action.hpp"
#include "../settler_job_status.hpp"
#include "../idle_mode.hpp"

using tasks::cancel_action;
using tasks::change_job_status;
using tasks::become_idle;

void do_hauling(entity_t &E, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name)
{
    if (ai.job_type_minor == JM_FIND_TIDY) {
        emit(item_claimed_message{(std::size_t)ai.target_id, true}); // Claim the item
        ai.current_path = find_path(pos, get_item_location(ai.target_id).get());
        if (!ai.current_path->success) {
            cancel_action(E, ai, stats, species, pos, name, "No route to item");
            return;
        }
        ai.job_type_minor = JM_GO_TO_TIDY;
        change_job_status(ai, name, "Find item to haul", true);
        return;
    }

    if (ai.job_type_minor == JM_GO_TO_TIDY) {
        tasks::try_path(E, ai, pos,
                        [] () {}, // Do nothing on success
                        [&ai, &name] () {
                            ai.current_path.reset();
                            ai.job_type_minor = JM_COLLECT_TIDY;
                            change_job_status(ai, name, "Collect item to haul", true);
                        }, // On arrival
                        [&E, &ai, &stats, &species, &pos, &name] () {
                            unclaim_by_id(ai.current_tool);
                            cancel_action(E, ai, stats, species, pos, name, "No route to item");
                        }
        );
        return;
    }

    if (ai.job_type_minor == JM_COLLECT_TIDY) {
        emit(pickup_item_message{ai.target_id, E.id});

        ai.job_type_minor = JM_FIND_STOCKPILE;
        change_job_status(ai, name, "Pathing to stockpile", true);
        return;
    }

    if (ai.job_type_minor == JM_FIND_STOCKPILE) {
        ai.current_path = find_path(pos, position_t{ai.target_x, ai.target_y, ai.target_z});
        if (!ai.current_path->success) {
            cancel_action(E, ai, stats, species, pos, name, "No route to stockpile");
            return;
        }
        ai.job_type_minor = JM_GO_TO_STOCKPILE;
        change_job_status(ai, name, "Find appropriate stockpile", true);
        return;
    }

    if (ai.job_type_minor == JM_GO_TO_STOCKPILE) {
        tasks::try_path(E, ai, pos,
                        [] () {}, // Do nothing on success
                        [&ai, &name] () {
                            ai.current_path.reset();
                            ai.job_type_minor = JM_STORE_ITEM;
                            change_job_status(ai, name, "Hauling", true);
                        }, // On arrival
                        [&E, &ai, &stats, &species, &pos, &name] () {
                            unclaim_by_id(ai.current_tool);
                            cancel_action(E, ai, stats, species, pos, name, "No route to stockpile");
                        }
        );
        return;
    }

    if (ai.job_type_minor == JM_STORE_ITEM) {
        change_job_status(ai, name, "Dropping item", true);
        emit(drop_item_message{ai.target_id, ai.target_x, ai.target_y, ai.target_z});
        become_idle(E, ai, name);
        return;
    }
}
