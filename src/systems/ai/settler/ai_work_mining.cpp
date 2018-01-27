#include "stdafx.h"
#include "jobs_board.hpp"
#include "../../../components/position.hpp"
#include "ai_work_template.hpp"
#include "../../../components/ai_tags/ai_tag_work_mining.hpp"
#include "../../../global_assets/game_designations.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../mining_system.hpp"
#include "../../physics/topology_system.hpp"
#include "../inventory_system.hpp"
#include "../../damage/damage_system.hpp"
#include "../../../components/item_tags/item_digging_t.hpp"

namespace systems {
	namespace ai_mining {

		using namespace bengine;
		using namespace jobs_board;
		using namespace dijkstra;
		using namespace distance_map;
		using namespace mining_system;
		using namespace region;

		namespace jobs_board {
			void evaluate_mining(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				/*
				if (designations->mining.empty()) return; // No mining to do

				auto pick_distance = pick_map.get(mapidx(pos));
				if (pick_distance > MAX_DIJSTRA_DISTANCE - 1) return; // No pick available

				const auto idx = mapidx(pos);
				if (mining_map[idx] == std::numeric_limits<uint8_t>::max()) return; // No mining to do
				const int distance = mining_map[idx] + pick_distance;

				board.insert(std::make_pair(distance, jt));
				*/
			}
		}

		static bool first_run = true;

		void run(const double &duration_ms) {
			if (first_run) {
				first_run = false;
				register_job_offer<ai_tag_work_miner>(jobs_board::evaluate_mining);
			}

			/*
			ai_work_template<ai_tag_work_miner> work;

			work.do_ai([&work](entity_t &e, ai_tag_work_miner &m, ai_tag_my_turn_t &t, position_t &pos) {
				std::cout << "BEGIN MINING TASK for " << e.id << "\n";
				work.set_status(e, "Mining");
				if (m.step == ai_tag_work_miner::mining_steps::GET_PICK) {
					// Do I already have a pick?
					bool have_pick = false;
					each<item_t, item_carried_t>([&e, &have_pick](entity_t &E, item_t &i, item_carried_t &ic) {
						if (ic.carried_by == e.id && i.type == TOOL_DIGGING) have_pick = true;
					});
					if (have_pick) {
						std::cout << "Skipping get pick - we already have one!\n";
						m.step = ai_tag_work_miner::mining_steps::GOTO_SITE;
					}

					std::cout << "GET PICK\n";
					work.folllow_path(pick_map, pos, e, [&e, &work]() {
						// On cancel
						std::cout << "GET PICK - CANCEL\n";
						work.cancel_work_tag(e);
						return;
					}, [&e, &pos, &m, &work] {
						// On success
						std::cout << "GET PICK - SUCCESS\n";
						work.pickup_tool<item_digging_t>(e, pos, m.current_pick, [&e, &work]() {
							// On cancel
							work.cancel_work_tag(e);
							return;
						}, [&m]() {
							// On success
							m.step = ai_tag_work_miner::mining_steps::GOTO_SITE;
							return;
						});
					});
				}
				else if (m.step == ai_tag_work_miner::mining_steps::GOTO_SITE) {
					std::cout << "Moving towards mining target\n";
					const auto idx = mapidx(pos.x, pos.y, pos.z);
					if (mining_map[idx] == 0) {
						// We're at a minable site
						m.step = ai_tag_work_miner::mining_steps::DIG;
						return;
					}
					if (idx == std::numeric_limits<uint8_t>::max()) {
						// There's nothing to do - someone else must have done it.
						std::cout << "Cancelling because of lack of mining tasks\n";
						m.step = ai_tag_work_miner::mining_steps::DROP_TOOLS;
						return;
					}

					int current_direction = 0;
					uint8_t min_value = std::numeric_limits<uint8_t>::max();
					if (mining_map[mapidx(pos.x, pos.y - 1, pos.z)] < min_value && flag(idx, CAN_GO_NORTH)) {
						min_value = mining_map[mapidx(pos.x, pos.y - 1, pos.z)];
						current_direction = 1;
					}
					if (mining_map[mapidx(pos.x, pos.y + 1, pos.z)] < min_value && flag(idx, CAN_GO_SOUTH)) {
						min_value = mining_map[mapidx(pos.x, pos.y + 1, pos.z)];
						current_direction = 2;
					}
					if (mining_map[mapidx(pos.x - 1, pos.y, pos.z)] < min_value && flag(idx, CAN_GO_WEST)) {
						min_value = mining_map[mapidx(pos.x - 1, pos.y, pos.z)];
						current_direction = 3;
					}
					if (mining_map[mapidx(pos.x + 1, pos.y, pos.z)] < min_value && flag(idx, CAN_GO_EAST)) {
						min_value = mining_map[mapidx(pos.x + 1, pos.y, pos.z)];
						current_direction = 4;
					}
					if (mining_map[mapidx(pos.x, pos.y, pos.z - 1)] < min_value && flag(idx, CAN_GO_DOWN)) {
						min_value = mining_map[mapidx(pos.x, pos.y, pos.z - 1)];
						current_direction = 5;
					}
					if (mining_map[mapidx(pos.x, pos.y, pos.z + 1)] < min_value && flag(idx, CAN_GO_UP)) {
						min_value = mining_map[mapidx(pos.x, pos.y, pos.z + 1)];
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
					case 1: --destination.y; break;
					case 2: ++destination.y; break;
					case 3: --destination.x; break;
					case 4: ++destination.x; break;
					case 5: --destination.z; break;
					case 6: ++destination.z; break;
					}
					movement::move_to(e.id, destination);
					std::cout << "Emitted entity movement - " << e.id << "\n";

					return;
				}
				else if (m.step == ai_tag_work_miner::mining_steps::DIG) {
					std::cout << "DIG\n";
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
						call_home("AI", "Mining", std::to_string(target_operation));

						if (target_operation > 0) {
							topology::perform_mining( target_idx, designations->mining[target_idx], static_cast<int>(pos.x), static_cast<int>(pos.y), pos.z );
							designations->mining.erase(target_idx);
							mining_system::mining_map_changed();
						}
						m.step = ai_tag_work_miner::mining_steps::DROP_TOOLS;
						return;
					}
					else {
						// Failed!
						if (skill_check == CRITICAL_FAIL) damage_system::inflict_damage(damage_system::inflict_damage_message{ e.id, 1, "Mining Accident" });
						return;
					}

					return;
				}
				else if (m.step == ai_tag_work_miner::mining_steps::DROP_TOOLS) {
					// Is there more mining to do?
					const auto idx = mapidx(pos.x, pos.y, pos.z);
					if (mining_map[idx] < std::numeric_limits<uint8_t>::max()) {
						std::cout << "Keep mining...\n";
						m.step = ai_tag_work_miner::mining_steps::GOTO_SITE;
						return;
					}

					std::cout << "Mining - drop tools, pick ID# " << m.current_pick << "\n";
					inventory_system::drop_item(m.current_pick, pos.x, pos.y, pos.z );
					distance_map::refresh_pick_map();
					work.cancel_work_tag(e);
					std::cout << "Finished MINING for " << e.id << "\n";
					return;
				}
			});
			*/
		}
	}
}