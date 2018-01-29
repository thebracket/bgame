#include "stdafx.h"
#include "ai_work_lumberjack.hpp"
#include "jobs_board.hpp"
#include "../../../planet/region/region.hpp"
#include "../distance_map_system.hpp"
#include "../../../global_assets/game_designations.hpp"
#include "../../../bengine/geometry.hpp"
#include "ai_work_template.hpp"
#include "../../../components/ai_tags/ai_tag_work_lumberjack.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../damage/damage_system.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "../../gui/particle_system.hpp"
#include "../../../components/item_tags/item_chopping_t.hpp"
#include "../../../components/name.hpp"
#include "../../../components/designated_lumberjack.hpp"
#include "../../../components/claimed_t.hpp"
#include "../../helpers/inventory_assistant.hpp"

using namespace bengine;
using namespace jobs_board;
using namespace region;
using namespace systems::distance_map;
using namespace systems::dijkstra;

namespace systems {
	namespace ai_work_lumberjack {

		using namespace jobs_board;

		namespace jobs_board {
			void evaluate_lumberjacking(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (designations->chopping.empty()) return; // Nothing to cut down

				if (e.component<designated_lumberjack_t>() == nullptr) return; // Not a lumberjack

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

				board.insert(std::make_pair(distance, jt));
			}
		}

		bool first_run = true;

		void run(const double &duration_ms) {
			if (first_run) {
				first_run = false;
				register_job_offer<ai_tag_work_lumberjack>(jobs_board::evaluate_lumberjacking);
			}

			ai_work_template<ai_tag_work_lumberjack> work;
			work.do_ai([&work](entity_t &e, ai_tag_work_lumberjack &lj, ai_tag_my_turn_t &t, position_t &pos) {
				work.set_status(e, "Lumberjacking");
				if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::GET_AXE) {
					// Do I already have an axe?
					bool have_pick = false;
					each<item_t, item_carried_t, claimed_t, item_chopping_t>([&e, &have_pick](entity_t &E, item_t &i, item_carried_t &ic, claimed_t &claimed, item_chopping_t &chop) {
						if (ic.carried_by == e.id) have_pick = true;
					});
					if (have_pick) {
						std::cout << "Skipping get axe - we already have one!\n";
						lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
						return;
					}
					else {
						// We need to fetch the axe
						std::size_t axe_id = -1;
						each<item_t, claimed_t, item_chopping_t>([&e, &axe_id](entity_t &E, item_t &i, claimed_t &claimed, item_chopping_t &chop) {
							if (claimed.claimed_by == e.id) axe_id = E.id;
						});
						if (axe_id == -1) {
							std::cout << "Unable to find claimed axe. Bug!\n";
							work.cancel_work_tag(e);
							return;
						}

						lj.axe_id = axe_id;
						auto axe_pos = inventory::get_item_location(axe_id);
						if (axe_pos) {
							lj.current_path = find_path(pos, *axe_pos);
							if (lj.current_path->success) {
								lj.step = ai_tag_work_lumberjack::lumberjack_steps::FETCH_AXE;
								return;
							}
							else {
								work.cancel_work_tag(e);
								return;
							}
						}
						else {
							work.cancel_work_tag(e);
							return;
						}
					}
				}
				else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::FETCH_AXE) {
					work.follow_path(lj, pos, e, [&lj]() {
						// Cancel
						lj.current_path.reset();
						lj.step = ai_tag_work_lumberjack::lumberjack_steps::GET_AXE;
						return;
					}, [&lj, &e]() {
						// Success
						lj.current_path.reset();
						lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;

						inventory_system::pickup_item(lj.axe_id, e.id);
						return;
					});
					return;
				}
				else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE) {
					//std::cout << "LJ: Find Tree\n";
					// Check that we're still a go
					if (designations->chopping.empty()) {
						// There is no tree - cancel
						//std::cout << "LJ: Give up - drop axe\n";
						work.cancel_work_tag(e);
						return;
					}

					// Find a tree position
					std::set<std::size_t> claimed_trees;
					each<ai_tag_work_lumberjack>([&claimed_trees, &e](entity_t &E, ai_tag_work_lumberjack &LJ) {
						if (e.id != E.id && LJ.target_tree > 0) claimed_trees.insert(E.id);
					});

					std::size_t tree_id = 0;
					float distance = std::numeric_limits<float>::max();
					for (auto it = designations->chopping.begin(); it != designations->chopping.end(); ++it) {
						auto dupe_finder = claimed_trees.find(it->first);
						if (dupe_finder == claimed_trees.end()) {
							const float d = distance3d(pos.x, pos.y, pos.z, it->second.x, it->second.y, it->second.z);
							if (d < distance) {
								tree_id = it->first;
								distance = d;
							}
						}
					}

					if (tree_id == 0) {
						//std::cout << "No tree - cancel\n";
						work.cancel_work_tag(e);
						return;
					}

					auto f = designations->chopping.find(tree_id);

					position_t tree_pos = f->second;
					lj.target_tree = f->first;
					lj.target_x = tree_pos.x;
					lj.target_y = tree_pos.y;
					lj.target_z = tree_pos.z;

					std::array<position_t, 4> target;
					target[0] = position_t{ tree_pos.x, tree_pos.y - 1, tree_pos.z };
					target[1] = position_t{ tree_pos.x, tree_pos.y + 1, tree_pos.z };
					target[2] = position_t{ tree_pos.x - 1, tree_pos.y, tree_pos.z };
					target[3] = position_t{ tree_pos.x + 1, tree_pos.y, tree_pos.z };

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
						//std::cout << "No path - cancel\n";
						work.cancel_work_tag(e);
						return;
					}
					else {
						lj.step = ai_tag_work_lumberjack::lumberjack_steps::GOTO_TREE;
						return;
					}
				}
				else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::GOTO_TREE) {
					//std::cout << "Go to tree\n";
					// Check that it is still a valid tree
					auto tree_finder = designations->chopping.find(lj.target_tree);
					if (tree_finder == designations->chopping.end()) {
						lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
						return;
					}

					// Go there
					work.follow_path(lj, pos, e, [&lj]() {
						// Cancel
						//std::cout << "Go to tree - cancel\n";
						lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
						return;
					}, [&lj]() {
						// We've arrived
						//std::cout << "Arrive at tree\n";
						lj.current_path.reset();
						lj.step = ai_tag_work_lumberjack::lumberjack_steps::CHOP;
						return;
					});
					return;
				}
				else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::CHOP) {
					//std::cout << "Chop tree\n";
					//std::cout << "Chop\n";
					// Check that it is still a valid tree
					auto tree_finder = designations->chopping.find(lj.target_tree);
					if (tree_finder == designations->chopping.end()) {
						lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
						return;
					}

					auto stats = e.component<game_stats_t>();
					if (!stats) {
						work.cancel_work_tag(e);
						return;
					}

					auto skill_check = skill_roll(e.id, *stats, rng, "Lumberjacking", DIFICULTY_TOUGH);

					if (skill_check >= SUCCESS) {
						call_home("AI", "Lumberjack");

						// Tree is going down!
						int number_of_logs = 0;
						int tree_idx = 0;
						int lowest_z = 1000;
						for (int z = 0; z<REGION_DEPTH; ++z) {
							for (int y = 0; y<REGION_HEIGHT; ++y) {
								for (int x = 0; x<REGION_WIDTH; ++x) {
									const auto idx = mapidx(x, y, z);
									if (tree_id(idx) == lj.target_tree) {
										if (z < lowest_z) {
											lowest_z = z;
											tree_idx = idx;
										}

										make_open_space(idx);
										tile_calculate(x, y, z);
										++number_of_logs;
										particles::block_destruction_effect(x, y, z, 0.0f, 1.0f, 0.0f, particles::PARTICLE_LUMBERJACK);
										chunks::mark_chunk_dirty_by_tileidx(mapidx(x, y, z));
									}
								}
							}
						}
						make_floor(tree_idx);
						tile_calculate(lj.target_x, lj.target_y, lj.target_z);
						auto [tx, ty, tz] = idxmap(tree_idx);

						// Spawn wooden logs
						number_of_logs = (number_of_logs / 20) + 1;
						for (int i = 0; i<number_of_logs; ++i) {
							std::cout << "Spawning wood\n";
							std::string cname = "";
							auto name = e.component<name_t>();
							if (name) cname = name->first_name + std::string(" ") + name->last_name;
							spawn_item_on_ground(tx, ty, tz, "wood_log", get_material_by_tag("wood"), 3, 100, e.id, cname);
						}

						// Update pathing
						for (int Z = -2; Z<10; ++Z) {
							for (int Y = -10; Y<10; ++Y) {
								for (int X = -10; X<10; ++X) {
									tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
									//tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
								}
							}
						}

						// Remove the tree from the designations list
						designations->chopping.erase(lj.target_tree);

						// Change status to drop axe or continue
						work.cancel_work_tag(e);
						return;

					}
					else if (skill_check == CRITICAL_FAIL) {
						// Damage yourself
						damage_system::inflict_damage(damage_system::inflict_damage_message{ e.id, 1, "Lumberjacking Accident" });
					}
					return;
				}
			});
		}
	}
}