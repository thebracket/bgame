#include "architecture_work.hpp"
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
#include "../../../components/item.hpp"
#include "../world_queries.hpp"
#include "../settler_sleep.hpp"
#include "../settler_wander.hpp"
#include "../settler_move_to.hpp"
#include "../../../raws/plants.hpp"
#include "../../distance_map_system.hpp"
#include "../settler_move_to.hpp"

using namespace rltk;
using tasks::become_idle;
using tasks::change_settler_glyph;
using tasks::change_job_status;
using tasks::drop_current_tool;
using tasks::cancel_action;
using tasks::follow_path;
using tasks::follow_result_t;
using tasks::calculate_initiative;

void do_architecture(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name)
{
    const int idx = mapidx(pos);

    if (ai.job_type_minor == JM_ARCHITECT_GOTOBLOCK) {
        const auto distance = blocks_map.distance_map[idx];
        if (distance >= MAX_DIJSTRA_DISTANCE) {
            cancel_action(e, ai, stats, species, pos, name, "No route to block");
            return;
        }
        if (distance == 0) {
            ai.job_type_minor = JM_ARCHITECT_COLLECTBLOCK;
            ai.job_status = "Collecting block";
            return;
        }
        position_t destination = blocks_map.find_destination(pos);
        move_to(e, pos, destination);
        return;
    }

    if (ai.job_type_minor == JM_ARCHITECT_COLLECTBLOCK) {
        std::size_t block_id = 0;
        each<item_t, position_t>([&pos, &block_id] (entity_t &E, item_t &i, position_t &block) {
            if (block == pos && i.claimed==false && i.item_tag=="block") {
                block_id = E.id;
            }
        });
        if (block_id == 0) {
            cancel_action(e, ai, stats, species, pos, name, "No available block");
            return;
        }
        std::cout << "Collected block\n";
        emit(item_claimed_message{block_id, true});
        emit(pickup_item_message{block_id, e.id});
        emit(blocks_changed_message{});
        ai.current_tool = block_id;
        ai.job_type_minor = JM_ARCHITECT_GOTOSITE;
        ai.job_status = "Going to building site";
        return;
    }

    if (ai.job_type_minor == JM_ARCHITECT_GOTOSITE) {
        const auto distance = architecure_map.distance_map[idx];
        if (distance >= MAX_DIJSTRA_DISTANCE) {
            cancel_action(e, ai, stats, species, pos, name, "No available task");
            return;
        }
        if (distance == 0) {
            ai.job_type_minor = JM_ARCHITECT_BUILD;
            ai.job_status = "Building";
            return;
        }
        ai.target_x = pos.x;
        ai.target_y = pos.y;
        ai.target_z = pos.z;
        position_t destination = architecure_map.find_destination(pos);
        move_to(e, pos, destination);
        return;
    }

    if (ai.job_type_minor == JM_ARCHITECT_BUILD) {
        const int bidx = mapidx(pos);
        pos.x = ai.target_x;
        pos.y = ai.target_y;
        pos.z = ai.target_z;
        auto finder = designations->architecture.find(bidx);
        if (finder != designations->architecture.end()) {
            uint8_t build_type = finder->second;

            std::size_t material = 0;
            auto block_e = entity(ai.current_tool);
            if (block_e) {
                auto Item = block_e->component<item_t>();
                if (Item) {
                    material = Item->material;
                }
            }
            emit(destroy_item_message{ai.current_tool});
            ai.current_tool = 0;

            current_region->tile_flags[bidx].set(CONSTRUCTION);
            if (material > 0) current_region->tile_material[bidx] = material;

            if (build_type == 0) {
                // Wall
                current_region->solid[bidx] = true;
                current_region->tile_type[bidx] = tile_type::WALL;
            } else if (build_type == 1) {
                // Floor
                current_region->tile_type[bidx] = tile_type::FLOOR;
            } else if (build_type == 2) {
                // Up
                current_region->tile_type[bidx] = tile_type::STAIRS_UP;
            } else if (build_type == 3) {
                // Down
                current_region->tile_type[bidx] = tile_type::STAIRS_DOWN;
            } else if (build_type == 4) {
                // UpDown
                current_region->tile_type[bidx] = tile_type::STAIRS_UPDOWN;
            } else if (build_type == 5) {
                // Ramp
                current_region->tile_type[bidx] = tile_type::RAMP;
            } else if (build_type == 6) {
                // TODO: Bridge
            }

            int cx, cy, cz;
            std::tie(cx, cy, cz) = idxmap(bidx);
            current_region->tile_calculate(cx, cy, cz);
            for (int Z=-2; Z<3; ++Z) {
                for (int Y=-2; Y<3; ++Y) {
                    for (int X=-2; X<3; ++X) {
                        current_region->tile_calculate(cx + X, cy + Y, cz + Z);
                    }
                }
            }

            designations->architecture.erase(bidx);
            emit(architecture_changed_message{});
            emit(renderables_changed_message{});
            emit(map_changed_message{});
        } else {
            cancel_action(e, ai, stats, species, pos, name, "Task not found");
            return;
        }
        become_idle(e, ai, name);
        return;
    }
}
