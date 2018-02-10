#include "../../../components/position.hpp"
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
#include "templated_work_steps_t.hpp"
#include "../../../global_assets/rng.hpp"
#include "../architecture_system.hpp"

namespace systems {
	namespace ai_mining {

		using namespace bengine;
		using namespace jobs_board;
		using namespace mining_system;
		using namespace region;

		namespace jobs_board {
			void evaluate_mining(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				const auto designation = e.component<designated_miner_t>();
				if (!designation) return; // Not a miner - so no jobs for you!

				const auto idx = mapidx(pos);
				if (mining_map[idx].distance == std::numeric_limits<uint8_t>::max()) return; // No mining to do

				board.insert(std::make_pair(static_cast<int>(mining_map[idx].distance), jt));
			}
		}

		static const char * job_tag = "Mining";		
		static work::templated_work_steps_t<ai_tag_work_miner> work;

		inline void get_pick(entity_t &e, ai_tag_work_miner &m, ai_tag_my_turn_t &t, position_t &pos) {
			work::get_tool<item_digging_t, designated_miner_t, ai_tag_work_miner>(e, work, m, pos, 
				[&m]() {
					// On we have it
					m.step = ai_tag_work_miner::mining_steps::GOTO_SITE;
				}, 
				[&m]() { 
					// On need to find it
					m.step = ai_tag_work_miner::mining_steps::FETCH_PICK; 
				});
		}

		inline void fetch_pick(entity_t &e, ai_tag_work_miner &m, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(m, pos, e, [&m]() {
				// Cancel
				m.current_path.reset();
				m.step = ai_tag_work_miner::mining_steps::GET_PICK;
				return;
			}, [&m, &e]() {
				// Success
				m.current_path.reset();
				m.step = ai_tag_work_miner::mining_steps::GOTO_SITE;

				inventory_system::pickup_item(m.tool_id, e.id);
				return;
			});
		}

		inline void goto_site(entity_t &e, ai_tag_work_miner &m, ai_tag_my_turn_t &t, position_t &pos) {
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
			}
		}

		inline void dig(entity_t &e, ai_tag_work_miner &m, ai_tag_my_turn_t &t, position_t &pos) {
			std::cout << "Dig !\n";
			const auto idx = mapidx(pos.x, pos.y, pos.z);
			if (mining_map[idx].distance != 0) {
				m.step = ai_tag_work_miner::mining_steps::GOTO_SITE;
				return;
			}

			if (work::skill_check_or_damage<ai_tag_work_miner>(e, "Mining", DIFICULTY_TOUGH, work, 1, "Mining Accident")) {
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
				architecture_system::architecture_map_changed();
				work.cancel_work_tag(e);
			}
		}

		void dispatch(entity_t &e, ai_tag_work_miner &m, ai_tag_my_turn_t &t, position_t &pos) {
			switch (m.step) {
			case ai_tag_work_miner::mining_steps::GET_PICK: get_pick(e, m, t, pos); break;
			case ai_tag_work_miner::mining_steps::FETCH_PICK: fetch_pick(e, m, t, pos); break;
			case ai_tag_work_miner::mining_steps::GOTO_SITE: goto_site(e, m, t, pos); break;
			case ai_tag_work_miner::mining_steps::DIG: dig(e, m, t, pos); break;
			}
		}

		void run(const double &duration_ms) {
			work.do_work(jobs_board::evaluate_mining, dispatch, job_tag);
		}
	}
}
