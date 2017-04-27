#include "ai_work_mining.hpp"
#include "../distance_map_system.hpp"
#include "../../../components/ai_tags/ai_tag_work_mining.hpp"
#include "../../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../../messages/inventory_changed_message.hpp"
#include "../../../components/item.hpp"
#include "../../../components/game_stats.hpp"
#include "../../../utils/telemetry.hpp"
#include "../../../messages/map_dirty_message.hpp"
#include "../../../messages/entity_moved_message.hpp"
#include "../../../messages/inflict_damage_message.hpp"
#include "../../../messages/perform_mining.hpp"
#include "../mining_system.hpp"
#include "ai_work_template.hpp"
#include "../../../main/game_designations.hpp"
#include "job_board.hpp"

namespace jobs_board {
    void evaluate_mining(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
        if (designations->mining.empty()) return; // No mining to do

        auto pick_distance = pick_map.get(mapidx(pos));
        if (pick_distance > MAX_DIJSTRA_DISTANCE-1) return; // No pick available

        const auto idx = mapidx(pos);
        const int distance = mining_map[idx] + pick_distance;

        board.insert(std::make_pair(distance, jt));
    }
}

void ai_work_mining::configure() {
    jobs_board::register_job_offer<ai_tag_work_miner>(jobs_board::evaluate_mining);
}

void ai_work_mining::update(const double duration_ms) {
    ai_work_template<ai_tag_work_miner> work;
    
    work.do_ai([this, &work] (entity_t &e, ai_tag_work_miner &m, ai_tag_my_turn_t &t, position_t &pos) {
        if (m.step == ai_tag_work_miner::mining_steps::GET_PICK) {
            work.folllow_path(pick_map, pos, e, [&e]() {
                // On cancel
                delete_component<ai_tag_work_miner>(e.id);
                return;
            }, [&e, this, &pos, &m, &work] {
                // On success
                work.pickup_tool<axemap_changed_message>(e, pos, TOOL_DIGGING, m.current_pick, [&e]() {
                    // On cancel
                    delete_component<ai_tag_work_miner>(e.id);
                    return;
                }, [&m]() {
                    // On success
                    m.step = ai_tag_work_miner::mining_steps::GOTO_SITE;
                    return;
                });
            });
        } else if (m.step == ai_tag_work_miner::mining_steps::GOTO_SITE) {
            std::cout << "Moving towards mining target\n";
            const auto idx = mapidx(pos.x, pos.y, pos.z);
            if (mining_map[idx]==0) {
                // We're at a minable site
                m.step = ai_tag_work_miner::mining_steps::DIG;
                return;
            }

            int current_direction = 0;
            uint8_t min_value = std::numeric_limits<uint8_t>::max();
            if (mining_map[mapidx(pos.x, pos.y-1, pos.z)] < min_value && current_region->tile_flags[idx].test(CAN_GO_NORTH)) {
                min_value = mining_map[mapidx(pos.x, pos.y-1, pos.z)];
                current_direction = 1;
            }
            if (mining_map[mapidx(pos.x, pos.y+1, pos.z)] < min_value && current_region->tile_flags[idx].test(CAN_GO_SOUTH)) {
                min_value = mining_map[mapidx(pos.x, pos.y+1, pos.z)];
                current_direction = 2;
            }
            if (mining_map[mapidx(pos.x-1, pos.y, pos.z)] < min_value && current_region->tile_flags[idx].test(CAN_GO_WEST)) {
                min_value = mining_map[mapidx(pos.x-1, pos.y, pos.z)];
                current_direction = 3;
            }
            if (mining_map[mapidx(pos.x+1, pos.y, pos.z)] < min_value && current_region->tile_flags[idx].test(CAN_GO_EAST)) {
                min_value = mining_map[mapidx(pos.x+1, pos.y, pos.z)];
                current_direction = 4;
            }
            if (mining_map[mapidx(pos.x, pos.y, pos.z-1)] < min_value && current_region->tile_flags[idx].test(CAN_GO_DOWN)) {
                min_value = mining_map[mapidx(pos.x, pos.y, pos.z-1)];
                current_direction = 5;
            }
            if (mining_map[mapidx(pos.x, pos.y, pos.z+1)] < min_value && current_region->tile_flags[idx].test(CAN_GO_UP)) {
                min_value = mining_map[mapidx(pos.x, pos.y, pos.z+1)];
                current_direction = 6;
            }

            if (current_direction == 0) {
                std::cout << "Direction 0 - drop tools\n";
                m.step = ai_tag_work_miner::mining_steps::DROP_TOOLS;
                return;
            }

            std::cout << "Direction: " << current_direction << "\n";
            position_t destination = pos;
            switch (current_direction) {
                case 1 : --destination.y; break;
                case 2 : ++destination.y; break;
                case 3 : --destination.x; break;
                case 4 : ++destination.x; break;
                case 5 : --destination.z; break;
                case 6 : ++destination.z; break;
            }
            emit_deferred(entity_wants_to_move_message{e.id, destination});
            std::cout << "Emitted entity movement - " << e.id << "\n";

            return;
        } else if (m.step == ai_tag_work_miner::mining_steps::DIG) {
            auto stats = e.component<game_stats_t>();
            if (!stats) {
                m.step = ai_tag_work_miner::mining_steps::DROP_TOOLS;
                return;
            }
            auto skill_check = skill_roll(e.id, *stats, rng, "Mining", DIFICULTY_TOUGH);
            if (skill_check >= SUCCESS) {
                // Determine the digging target from here
                // Make a skill roll, and if successful complete the action
                // When complete, move to dropping the pick
                const auto idx = mapidx(pos.x, pos.y, pos.z);
                const int target_idx = mining_targets[idx];
                const int target_operation = designations->mining[target_idx];
                call_home("mining", std::to_string(target_operation));

                if (target_operation > 0) {
                    emit(perform_mining_message{mining_targets[idx], designations->mining[target_idx], static_cast<int>(pos.x), static_cast<int>(pos.y), pos.z});
                    designations->mining.erase(target_idx);
                }
                m.step = ai_tag_work_miner::mining_steps::DROP_TOOLS;
                return;
            } else {
                // Failed!
                if (skill_check == CRITICAL_FAIL) emit_deferred(inflict_damage_message{e.id, 1, "Mining Accident"});
                return;
            }

            return;
        } else if (m.step == ai_tag_work_miner::mining_steps::DROP_TOOLS) {
            emit(drop_item_message{m.current_pick, pos.x, pos.y, pos.z});
            emit(pickmap_changed_message{});
            delete_component<ai_tag_work_miner>(e.id);
            return;
        }
    });
}
