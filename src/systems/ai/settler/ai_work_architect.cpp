#include "ai_work_architect.hpp"
#include "../../../components/ai_tags/ai_tag_work_architect.hpp"
#include "job_board.hpp"
#include "../distance_map_system.hpp"
#include "ai_work_template.hpp"
#include "../../../main/game_designations.hpp"
#include "../../../messages/map_dirty_message.hpp"
#include "../../../components/item.hpp"
#include "../../../components/bridge.hpp"
#include "../../../components/receives_signal.hpp"
#include "../../../utils/telemetry.hpp"

namespace jobs_board {
    void evaluate_architecture(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
        auto blocks_distance = blocks_map.get(mapidx(pos));
        if (blocks_distance > MAX_DIJSTRA_DISTANCE-1) return; // Nothing to harvest

        auto build_distance = architecure_map.get(mapidx(pos));
        if (build_distance > MAX_DIJSTRA_DISTANCE-1) return; // Nothing to harvest

        board.insert(std::make_pair(blocks_distance + build_distance, jt));
    }
}

void ai_work_architect::configure() {
    // Register with the jobs board
    jobs_board::register_job_offer<ai_tag_work_architect>(jobs_board::evaluate_architecture);
}

void ai_work_architect::update(const double duration_ms) {
    ai_work_template<ai_tag_work_architect> work;
    work.do_ai([this, &work] (entity_t &e, ai_tag_work_architect &a, ai_tag_my_turn_t &t, position_t &pos) {
        if (a.step == ai_tag_work_architect::architect_steps::GOTO_BLOCK) {
            work.folllow_path(blocks_map, pos, e, [&e, &work] () {
                // Cancel
                work.cancel_work_tag(e);
            }, [&a] () {
                // Success
                a.step = ai_tag_work_architect::architect_steps::COLLECT_BLOCK;
            });
            return;
        } else if (a.step == ai_tag_work_architect::architect_steps::COLLECT_BLOCK) {
            std::size_t block_id = 0;
            each<item_t, position_t>([&pos, &block_id] (entity_t &E, item_t &i, position_t &block) {
                if (block == pos && i.claimed==false && i.item_tag=="block") {
                    block_id = E.id;
                }
            });
            if (block_id == 0) {
                work.cancel_work_tag(e);
                return;
            }
            std::cout << "Collected block\n";
            emit(item_claimed_message{block_id, true});
            emit(pickup_item_message{block_id, e.id});
            emit(blocks_changed_message{});
            a.current_tool = block_id;
            a.step = ai_tag_work_architect::architect_steps::GOTO_SITE;
            return;
        } else if (a.step == ai_tag_work_architect::architect_steps::GOTO_SITE) {
            const int idx = mapidx(pos);
            const auto distance = architecure_map.get(idx);
            if (distance >= MAX_DIJSTRA_DISTANCE) {
                emit(drop_item_message{a.current_tool, pos.x, pos.y, pos.z});
                work.cancel_work_tag(e);
                return;
            }
            if (distance < 2) {
                a.step = ai_tag_work_architect::architect_steps::BUILD;
                return;
            }
            position_t destination = architecure_map.find_destination(pos);
            emit(entity_wants_to_move_message{e.id, destination});
            return;
        } else if (a.step == ai_tag_work_architect::architect_steps::BUILD) {
            int bidx = 0;

            // Are we standing on the build site, adjacent to it?
            if (designations->architecture.find(mapidx(pos)) != designations->architecture.end()) bidx = mapidx(pos);
            if (bidx == 0 && designations->architecture.find(mapidx(pos.x-1, pos.y, pos.z)) != designations->architecture.end()) bidx = mapidx(pos.x-1, pos.y, pos.z);
            if (bidx == 0 && designations->architecture.find(mapidx(pos.x+1, pos.y, pos.z)) != designations->architecture.end()) bidx = mapidx(pos.x+1, pos.y, pos.z);
            if (bidx == 0 && designations->architecture.find(mapidx(pos.x, pos.y-1, pos.z)) != designations->architecture.end()) bidx = mapidx(pos.x, pos.y-1, pos.z);
            if (bidx == 0 && designations->architecture.find(mapidx(pos.x, pos.y+1, pos.z)) != designations->architecture.end()) bidx = mapidx(pos.x, pos.y+1, pos.z);

            auto finder = designations->architecture.find(bidx);
            if (finder != designations->architecture.end()) {
                uint8_t build_type = finder->second;

                std::size_t material = 0;
                auto block_e = entity(a.current_tool);
                if (block_e) {
                    auto Item = block_e->component<item_t>();
                    if (Item) {
                        material = Item->material;
                    }
                }
                emit(destroy_item_message{a.current_tool});
                a.current_tool = 0;

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
                    current_region->tile_type[bidx] = tile_type::FLOOR;
                    // We need to iterate through the bridge tiles and see if it is done yet.
                    const auto bridge_id = current_region->bridge_id[bidx];
                    bool complete = true;
                    int bridge_idx = 0;
                    for (auto &id : current_region->bridge_id) {
                        if (id == bridge_id && bridge_idx != bidx && designations->architecture.find(bridge_idx) != designations->architecture.end()) {
                            complete = false;
                        }
                        ++bridge_idx;
                    }
                    if (complete) {
                        entity(bridge_id)->component<bridge_t>()->complete = true;
                        entity(bridge_id)->assign(receives_signal_t{});
                    }
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
                call_home("architecture", std::to_string(build_type));
            } else {
                emit(drop_item_message{a.current_tool, pos.x, pos.y, pos.z});
                work.cancel_work_tag(e);
                return;
            }
            work.cancel_work_tag(e);
            return;
        }
    });
}
