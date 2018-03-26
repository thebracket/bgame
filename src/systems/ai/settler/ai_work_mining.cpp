#include "stdafx.h"
#include "../../../global_assets/game_mining.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../mining_system.hpp"
#include "../../physics/topology_system.hpp"
#include "../inventory_system.hpp"
#include "templated_work_steps_t.hpp"
#include "../architecture_system.hpp"
#include "../../helpers/targeted_flow_map.hpp"

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

				board.insert(std::make_pair(10, jt));
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
			if (m.current_path)
			{
				// Follow the path
				work.follow_path(m, pos, e, [&m]() {
					// Cancel
					m.current_path.reset();
					// The current path is no longer valid, so we're going to make a new one.
					return;
				}, [&m]() {
					// Success
					m.current_path.reset();
					m.step = ai_tag_work_miner::mining_steps::DIG;
					return;
				});
			} else
			{
				// Find one
				const auto mine_search = mining_map->find_nearest_reachable_target(pos);
				if (mine_search.target == 0)
				{
					// There is no available path.
					work.cancel_work_tag(e);
				} else
				{
					// We have a path to follow now
					m.current_path = mine_search.path;
					m.target_tile = mine_search.target;
				}
			}
		}

		inline void dig(entity_t &e, ai_tag_work_miner &m, ai_tag_my_turn_t &t, position_t &pos) {
			//std::cout << "Dig !\n";
			const auto idx = mapidx(pos.x, pos.y, pos.z);
			if (!mining_map->is_target(idx) || mining_designations->mining_targets.find(m.target_tile) == mining_designations->mining_targets.end()) {
				m.step = ai_tag_work_miner::mining_steps::GOTO_SITE;
				return;
			}

			if (work::skill_check_or_damage<ai_tag_work_miner>(e, "Mining", DIFICULTY_TOUGH, work, 1, "Mining Accident")) {
				// Determine the digging target from here
				// Make a skill roll, and if successful complete the action
				// When complete, move to dropping the pick
				const int target_idx = m.target_tile;
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
