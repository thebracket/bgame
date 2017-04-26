#include "ai_work_lumberjack.hpp"
#include "../../../components/ai_tag_work_lumberjack.hpp"
#include "../../../components/ai_tag_my_turn.hpp"
#include "../distance_map_system.hpp"
#include "../../../messages/inventory_changed_message.hpp"
#include "../../../components/item.hpp"
#include "../path_finding.hpp"
#include "../../../components/game_stats.hpp"
#include "../../../messages/map_dirty_message.hpp"
#include "../../../messages/inflict_damage_message.hpp"
#include "ai_work_template.hpp"
#include "../../../main/game_designations.hpp"
#include "job_board.hpp"

namespace jobs_board {
    void evaluate_lumberjacking(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
        if (designations->chopping.empty()) return; // Nothing to cut down

        auto axe_distance = axe_map.get(mapidx(pos));
        if (axe_distance > MAX_DIJSTRA_DISTANCE-1) return; // No axe available

        // Evaluate the closest tree to chop
        std::size_t i = 0;
        float distance = std::numeric_limits<float>().max();
        std::size_t selected = 0;
        for (const auto &chop : designations->chopping) {
            const float d = distance3d(pos.x, pos.y, pos.z, chop.second.x, chop.second.y, chop.second.z);
            if (d < distance) {
                distance = d;
                selected = i;
            }
            ++i;
        }

        board.insert(std::make_pair(distance + axe_distance, jt));
    }
}

void ai_work_lumberjack::configure() {
    // Register with the jobs board
    jobs_board::register_job_offer<ai_tag_work_lumberjack>(jobs_board::evaluate_lumberjacking);
}

void ai_work_lumberjack::update(const double duration_ms)
{
    ai_work_template<ai_tag_work_lumberjack> work;
    work.do_ai([this, &work] (entity_t &e, ai_tag_work_lumberjack &lj, ai_tag_my_turn_t &t, position_t &pos) {
        if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::GET_AXE) {
            work.fetch_tool(axe_map, pos, e, [&e] () {
                // On cancel
                delete_component<ai_tag_work_lumberjack>(e.id);
                return;
            }, [&e, this, &pos, &lj, &work] {
                // On success
                work.pickup_tool<axemap_changed_message>(e, pos, TOOL_CHOPPING, lj.current_axe, [&e, &lj] () {
                    // On cancel
                    delete_component<ai_tag_work_lumberjack>(e.id);
                    return;
                }, [&lj] () {
                    // On success
                    lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
                });
            });
        } else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE) {
            // Check that we're still a go
            if (designations->chopping.empty()) {
                // There is no tree - cancel
                emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
                emit(axemap_changed_message{});
                delete_component<ai_tag_work_lumberjack>(e.id);
                return;
            }

            // Find a tree position
            position_t tree_pos = designations->chopping.begin()->second;
            lj.target_tree = designations->chopping.begin()->first;
            lj.target_x = tree_pos.x;
            lj.target_y = tree_pos.y;
            lj.target_z = tree_pos.z;

            std::array<position_t, 4> target;
            target[0] = position_t{ tree_pos.x, tree_pos.y-1, tree_pos.z };
            target[1] = position_t{ tree_pos.x, tree_pos.y+1, tree_pos.z };
            target[2] = position_t{ tree_pos.x-1, tree_pos.y, tree_pos.z };
            target[3] = position_t{ tree_pos.x+1, tree_pos.y, tree_pos.z };

            int n = 0;
            while (!lj.current_path && n<4) {
                lj.current_path = find_path(pos, target[n]);
                if (!lj.current_path->success) {
                    lj.current_path.reset();
                }
                ++n;
            }

            // Are we good to go?
            if (!lj.current_path) {
                // There is no path - cancel
                emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
                emit(axemap_changed_message{});
                delete_component<ai_tag_work_lumberjack>(e.id);
                return;
            } else {
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::GOTO_TREE;
                return;
            }
        } else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::GOTO_TREE) {
            work.follow_path(lj, pos, e, [&lj] () {
                // Cancel
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
                return;
            }, [&lj] () {
                // We've arrived
                lj.current_path.reset();
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::CHOP;
                return;
            });
        }else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::CHOP) {
            //std::cout << "Chop\n";

            auto stats = e.component<game_stats_t>();
            if (!stats) {
                emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
                emit(axemap_changed_message{});
                delete_component<ai_tag_work_lumberjack>(e.id);
                return;
            }

            auto skill_check = skill_roll(e.id, *stats, rng, "Lumberjacking", DIFICULTY_TOUGH);

            if (skill_check >= SUCCESS) {
                //call_home("tree_chop");
                // Tree is going down!
                int number_of_logs = 0;
                int tree_idx = 0;
                int lowest_z = 1000;
                for (int z=0; z<REGION_DEPTH; ++z) {
                    for (int y=0; y<REGION_HEIGHT; ++y) {
                        for (int x=0; x<REGION_WIDTH; ++x) {
                            const auto idx = mapidx(x,y,z);
                            if (current_region->tree_id[idx] == lj.target_tree) {
                                if (z < lowest_z) {
                                    lowest_z = z;
                                    tree_idx = idx;
                                }

                                current_region->solid[idx]=false;
                                current_region->opaque[idx]=false;
                                current_region->tile_flags[idx].reset(CAN_STAND_HERE);
                                current_region->tree_id[idx] = 0;
                                current_region->tile_type[idx] = tile_type::OPEN_SPACE;
                                current_region->tile_calculate(x,y,z);
                                ++number_of_logs;
                            }
                        }
                    }
                }
                current_region->tile_type[tree_idx] = tile_type::FLOOR;
                current_region->tile_flags[tree_idx].set(CAN_STAND_HERE);
                current_region->tile_calculate(lj.target_x, lj.target_y, lj.target_z);
                int tx,ty,tz;
                std::tie(tx,ty,tz) = idxmap(tree_idx);

                // Spawn wooden logs
                number_of_logs = (number_of_logs/20)+1;
                for (int i=0; i<number_of_logs; ++i) {
                    spawn_item_on_ground(tx, ty, tz, "wood_log", get_material_by_tag("wood"));
                }

                // Update pathing
                for (int Z=-2; Z<10; ++Z) {
                    for (int Y=-10; Y<10; ++Y) {
                        for (int X=-10; X<10; ++X) {
                            current_region->tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
                            current_region->tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
                        }
                    }
                }

                // Change status to drop axe or continue
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::DROP_TOOLS;

            } else if (skill_check == CRITICAL_FAIL) {
                // Damage yourself
                emit_deferred(inflict_damage_message{e.id, 1, "Lumberjacking Accident"});
            }
            return;
        } else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::DROP_TOOLS) {
            emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
            emit(axemap_changed_message{});
            delete_component<ai_tag_work_lumberjack>(e.id);
            return;
        }
    });
}

/*
void ai_work_lumberjack::update(const double duration_ms) {
    each<ai_tag_work_lumberjack, ai_tag_my_turn_t, position_t>([] (entity_t &e, ai_tag_work_lumberjack &lj, ai_tag_my_turn_t &t, position_t &pos) {
        delete_component<ai_tag_my_turn_t>(e.id); // It's not my turn anymore

        //std::cout << "Lumberjacking\n";

        if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::GET_AXE) {
            const auto d = axe_map.get(mapidx(pos));
            //std::cout << "Get axe - distance " << d << "\n";
            if (d > MAX_DIJSTRA_DISTANCE-1) {
                // Cancel the job, we can't do it
                delete_component<ai_tag_work_lumberjack>(e.id);
                return;
            } else if (d < 1) {
                //std::cout << "Picking up axe\n";

                // Pick up the axe
                std::size_t tool_id = 0;
                each<item_t>([&tool_id, &pos] (entity_t &axe, item_t &item) {
                    if (item.category.test(TOOL_CHOPPING)) return; // Not an axe

                    auto axe_pos = axe.component<position_t>();
                    if (axe_pos != nullptr && *axe_pos == pos) {
                        tool_id = axe.id;
                    } else {
                        auto stored = axe.component<item_stored_t>();
                        if (stored != nullptr) {
                            auto spos = entity(stored->stored_in)->component<position_t>();
                            if (spos != nullptr && *spos == pos) {
                                tool_id = axe.id;
                            }
                        }
                    }
                });
                if (tool_id > 0) {
                    lj.current_axe = tool_id;
                    emit(pickup_item_message{lj.current_axe, e.id});
                    emit(axemap_changed_message{});
                    lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
                } else {
                    //std::cout << "We failed to find the axe\n";
                    // We've failed to get the axe!
                    delete_component<ai_tag_work_lumberjack>(e.id);
                }
                return;
            } else {
                // Path towards the axe
                position_t destination = axe_map.find_destination(pos);
                emit_deferred(entity_wants_to_move_message{e.id, destination});
                return;
            }
        } else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE) {
            //std::cout << "Find tree\n";

            if (designations->chopping.empty()) {
                // There is no tree - cancel
                emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
                emit(axemap_changed_message{});
                delete_component<ai_tag_work_lumberjack>(e.id);
                return;
            }
            position_t tree_pos = designations->chopping.begin()->second;
            lj.target_tree = designations->chopping.begin()->first;
            lj.target_x = tree_pos.x;
            lj.target_y = tree_pos.y;
            lj.target_z = tree_pos.z;

            std::array<position_t, 4> target;
            target[0] = position_t{ tree_pos.x, tree_pos.y-1, tree_pos.z };
            target[1] = position_t{ tree_pos.x, tree_pos.y+1, tree_pos.z };
            target[2] = position_t{ tree_pos.x-1, tree_pos.y, tree_pos.z };
            target[3] = position_t{ tree_pos.x+1, tree_pos.y, tree_pos.z };

            int n = 0;
            while (!lj.current_path && n<4) {
                lj.current_path = find_path(pos, target[n]);
                if (!lj.current_path->success) {
                    lj.current_path.reset();
                }
                ++n;
            }

            if (!lj.current_path) {
                // There is no path - cancel
                emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
                emit(axemap_changed_message{});
                delete_component<ai_tag_work_lumberjack>(e.id);
                return;
            } else {
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::GOTO_TREE;
                return;
            }
        } else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::GOTO_TREE) {
            //std::cout << "Goto tree\n";

            if (!lj.current_path) {
                // No path, so we go back a step.
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
                return;
            }

            // We want to follow the path
            if (pos == lj.current_path->destination) {
                // We got there!
                lj.current_path.reset();
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::CHOP;
            } else {
                if (lj.current_path->steps.empty()) {
                    lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
                    return;
                }

                const position_t next_step = lj.current_path->steps.front();
                lj.current_path->steps.pop_front();
                if (next_step.x > 0 && next_step.x < REGION_WIDTH && next_step.y > 0 &&
                        next_step.y < REGION_HEIGHT && next_step.z > 0 && next_step.z < REGION_DEPTH
                    && current_region->tile_flags[mapidx(next_step)].test(CAN_STAND_HERE))
                {
                    emit_deferred(entity_wants_to_move_message{e.id, next_step});
                    emit_deferred(renderables_changed_message{});
                } else {
                    // We couldn't get there
                    lj.current_path.reset();
                    lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
                }
            }

            return;
        } else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::CHOP) {
            //std::cout << "Chop\n";

            auto stats = e.component<game_stats_t>();
            if (!stats) {
                emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
                emit(axemap_changed_message{});
                delete_component<ai_tag_work_lumberjack>(e.id);
                return;
            }

            auto skill_check = skill_roll(e.id, *stats, rng, "Lumberjacking", DIFICULTY_TOUGH);

            if (skill_check >= SUCCESS) {
                //call_home("tree_chop");
                // Tree is going down!
                int number_of_logs = 0;
                int tree_idx = 0;
                int lowest_z = 1000;
                for (int z=0; z<REGION_DEPTH; ++z) {
                    for (int y=0; y<REGION_HEIGHT; ++y) {
                        for (int x=0; x<REGION_WIDTH; ++x) {
                            const auto idx = mapidx(x,y,z);
                            if (current_region->tree_id[idx] == lj.target_tree) {
                                if (z < lowest_z) {
                                    lowest_z = z;
                                    tree_idx = idx;
                                }

                                current_region->solid[idx]=false;
                                current_region->opaque[idx]=false;
                                current_region->tile_flags[idx].reset(CAN_STAND_HERE);
                                current_region->tree_id[idx] = 0;
                                current_region->tile_type[idx] = tile_type::OPEN_SPACE;
                                current_region->tile_calculate(x,y,z);
                                ++number_of_logs;
                            }
                        }
                    }
                }
                current_region->tile_type[tree_idx] = tile_type::FLOOR;
                current_region->tile_flags[tree_idx].set(CAN_STAND_HERE);
                current_region->tile_calculate(lj.target_x, lj.target_y, lj.target_z);
                int tx,ty,tz;
                std::tie(tx,ty,tz) = idxmap(tree_idx);

                // Spawn wooden logs
                number_of_logs = (number_of_logs/20)+1;
                for (int i=0; i<number_of_logs; ++i) {
                    spawn_item_on_ground(tx, ty, tz, "wood_log", get_material_by_tag("wood"));
                }

                // Update pathing
                for (int Z=-2; Z<10; ++Z) {
                    for (int Y=-10; Y<10; ++Y) {
                        for (int X=-10; X<10; ++X) {
                            current_region->tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
                            current_region->tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
                        }
                    }
                }

                // Change status to drop axe or continue
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::DROP_TOOLS;

            } else if (skill_check == CRITICAL_FAIL) {
                // Damage yourself
                emit_deferred(inflict_damage_message{e.id, 1, "Lumberjacking Accident"});
            }
            return;
        } else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::DROP_TOOLS) {
            emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
            emit(axemap_changed_message{});
            delete_component<ai_tag_work_lumberjack>(e.id);
            return;
        }
    });
}
*/