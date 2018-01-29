#include "stdafx.h"
#include "jobs_board.hpp"
#include "../../../components/position.hpp"
#include "ai_work_template.hpp"
#include "../../../components/ai_tags/ai_tag_work_mining.hpp"
#include "../../../global_assets/game_mining.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../mining_system.hpp"
#include "../../physics/topology_system.hpp"
#include "../inventory_system.hpp"
#include "../../damage/damage_system.hpp"
#include "../../../components/item_tags/item_digging_t.hpp"
#include "../../../components/mining/designated_miner.hpp"
#include "../../../components/claimed_t.hpp"
#include "../../helpers/inventory_assistant.hpp"

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
				auto designation = e.component<designated_miner_t>();
				if (!designation) return; // Not a miner - so no jobs for you!

				const auto idx = mapidx(pos);
				if (mining_map[idx].distance == std::numeric_limits<uint8_t>::max()) return; // No mining to do

				board.insert(std::make_pair(mining_map[idx].distance, jt));
			}
		}

		static bool first_run = true;

		void run(const double &duration_ms) {
			if (first_run) {
				first_run = false;
				register_job_offer<ai_tag_work_miner>(jobs_board::evaluate_mining);
			}

			ai_work_template<ai_tag_work_miner> work;
			work.do_ai([&work](entity_t &e, ai_tag_work_miner &m, ai_tag_my_turn_t &t, position_t &pos) {
				std::cout << "BEGIN MINING TASK for " << e.id << "\n";
				work.set_status(e, "Mining");

				if (m.step == ai_tag_work_miner::mining_steps::GET_PICK) {
					// Do I already have a pick?
					bool have_pick = false;
					each<item_t, item_carried_t, claimed_t, item_digging_t>([&e, &have_pick](entity_t &E, item_t &i, item_carried_t &ic, claimed_t &claimed, item_digging_t &dig) {
						if (ic.carried_by == e.id) have_pick = true;
					});
					if (have_pick) {
						std::cout << "Skipping get pick - we already have one!\n";
						m.step = ai_tag_work_miner::mining_steps::GOTO_SITE;
						return;
					}
					else {
						// We need to fetch the pick
						std::size_t pick_id = -1;
						each<item_t, claimed_t, item_digging_t>([&e, &pick_id](entity_t &E, item_t &i, claimed_t &claimed, item_digging_t &dig) {
							if (claimed.claimed_by == e.id) pick_id = E.id;
						});
						if (pick_id == -1) {
							std::cout << "Unable to find claimed pick. Bug!\n";
							work.cancel_work_tag(e);
							return;
						}

						m.pick_id = pick_id;
						auto pick_pos = inventory::get_item_location(pick_id);
						if (pick_pos) {
							m.current_path = find_path(pos, *pick_pos);
							if (m.current_path->success) {
								m.step = ai_tag_work_miner::mining_steps::FETCH_PICK;
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
				else if (m.step == ai_tag_work_miner::mining_steps::FETCH_PICK) {
					work.follow_path(m, pos, e, [&m]() {
						// Cancel
						m.current_path.reset();
						m.step = ai_tag_work_miner::mining_steps::GET_PICK;
						return;
					}, [&m, &e]() {
						// Success
						m.current_path.reset();
						m.step = ai_tag_work_miner::mining_steps::GOTO_SITE;

						inventory_system::pickup_item(m.pick_id, e.id);
						return;
					});
					return;
				}
				else if (m.step == ai_tag_work_miner::mining_steps::GOTO_SITE) {
					const auto idx = mapidx(pos.x, pos.y, pos.z);
					if (mining_map[idx].distance == 0) {
						// We're at a minable site
						m.step = ai_tag_work_miner::mining_steps::DIG;
						return;
					}
					else if (mining_map[idx].distance == std::numeric_limits<uint8_t>::max()) {
						// There's nothing to do - someone else must have done it.
						std::cout << "Cancelling because of lack of mining tasks\n";
						work.cancel_work_tag(e);
						return;
					}
					else {
						// Path towards the work
						int current_direction = 0;
						uint8_t min_value = std::numeric_limits<uint8_t>::max();
						if (mining_map[mapidx(pos.x, pos.y - 1, pos.z)].distance < min_value && flag(idx, CAN_GO_NORTH)) {
							min_value = mining_map[mapidx(pos.x, pos.y - 1, pos.z)].distance;
							current_direction = 1;
						}
						if (mining_map[mapidx(pos.x, pos.y + 1, pos.z)].distance < min_value && flag(idx, CAN_GO_SOUTH)) {
							min_value = mining_map[mapidx(pos.x, pos.y + 1, pos.z)].distance;
							current_direction = 2;
						}
						if (mining_map[mapidx(pos.x - 1, pos.y, pos.z)].distance < min_value && flag(idx, CAN_GO_WEST)) {
							min_value = mining_map[mapidx(pos.x - 1, pos.y, pos.z)].distance;
							current_direction = 3;
						}
						if (mining_map[mapidx(pos.x + 1, pos.y, pos.z)].distance < min_value && flag(idx, CAN_GO_EAST)) {
							min_value = mining_map[mapidx(pos.x + 1, pos.y, pos.z)].distance;
							current_direction = 4;
						}
						if (mining_map[mapidx(pos.x, pos.y, pos.z - 1)].distance < min_value && flag(idx, CAN_GO_DOWN)) {
							min_value = mining_map[mapidx(pos.x, pos.y, pos.z - 1)].distance;
							current_direction = 5;
						}
						if (mining_map[mapidx(pos.x, pos.y, pos.z + 1)].distance < min_value && flag(idx, CAN_GO_UP)) {
							min_value = mining_map[mapidx(pos.x, pos.y, pos.z + 1)].distance;
							current_direction = 6;
						}

						if (current_direction == 0) {
							std::cout << "Direction 0 - drop tools\n";
							work.cancel_work_tag(e);
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
				}
				else if (m.step == ai_tag_work_miner::mining_steps::DIG) {
					std::cout << "Dig !\n";
					const auto idx = mapidx(pos.x, pos.y, pos.z);
					if (mining_map[idx].distance != 0) {
						m.step = ai_tag_work_miner::mining_steps::GOTO_SITE;
						return;
					}

					auto stats = e.component<game_stats_t>();
					if (!stats) {
						std::cout << "Cancelled because of missing stats.\n";
						work.cancel_work_tag(e);
						return;
					}

					auto skill_check = skill_roll(e.id, *stats, rng, "Mining", DIFICULTY_TOUGH);
					if (skill_check >= SUCCESS) {
						// Determine the digging target from here
						// Make a skill roll, and if successful complete the action
						// When complete, move to dropping the pick
						const auto idx = mapidx(pos.x, pos.y, pos.z);
						const int target_idx = mining_map[idx].target;
						const auto target_operation = mining_designations->mining_targets[target_idx];
						call_home("AI", "Mining", std::to_string(target_operation));

						topology::perform_mining(target_idx, target_operation, static_cast<int>(pos.x), static_cast<int>(pos.y), pos.z);
						mining_designations->mining_targets.erase(target_idx);
						mining_system::mining_map_changed();
						work.cancel_work_tag(e);
						return;
					}
					else {
						// Failed!
						if (skill_check == CRITICAL_FAIL) damage_system::inflict_damage(damage_system::inflict_damage_message{ e.id, 1, "Mining Accident" });
						return;
					}

					return;
				}
			});
		}
	}
}