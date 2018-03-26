#include "stdafx.h"
#include "ai_work_hunt.hpp"
#include "templated_work_steps_t.hpp"
#include "../distance_map_system.hpp"
#include "../../helpers/targeted_flow_map.hpp"
#include "../../helpers/weapons_helper.hpp"

namespace systems {
	namespace ai_hunt {

		using namespace bengine;
		using namespace jobs_board;

		namespace jobs_board {

			void evaluate_hunting(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (e.component<designated_hunter_t>() == nullptr) return; // Not a hunter
				if (distance_map::hunting_map->targets.empty()) return; // Nothing to hunt

				board.insert(std::make_pair(20, jt));
			}
		}

		static const char * job_tag = "Hunting";
		static work::templated_work_steps_t<ai_tag_work_hunting> work;

		inline void check_status(entity_t &e, ai_tag_work_hunting &a, ai_tag_my_turn_t &t, position_t &pos) {
			if (weapons::shooting_range(e, pos) < 1)
			{
				// Cancel the job - no weapon!
				work.cancel_work_tag(e);
			}
			a.step = ai_tag_work_hunting::hunting_steps::FIND_TARGET;
			a.current_path.reset();
		}

		inline void find_target(entity_t &e, ai_tag_work_hunting &a, ai_tag_my_turn_t &t, position_t &pos) {
			const auto finder = distance_map::hunting_map->find_nearest_reachable_target(pos);
			if (finder.target == 0)
			{
				work.cancel_work_tag(e);
			} else
			{
				a.current_path = finder.path;
				a.step = ai_tag_work_hunting::hunting_steps::HUNT;
			}
		}

		inline void hunt(entity_t &e, ai_tag_work_hunting &a, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(a, pos, e, [&e, &a]()
			{
				// Cancel
				a.current_path.reset();
				work.cancel_work_tag(e);
			}, [&a, &e]()
			{
				// Success
				a.current_path.reset();
				work.cancel_work_tag(e);
			});
		}


		void dispatch(entity_t &e, ai_tag_work_hunting &h, ai_tag_my_turn_t &t, position_t &pos) {
			switch (h.step)
			{
			case ai_tag_work_hunting::hunting_steps::CHECK_STATUS: check_status(e, h, t, pos); break;
			case ai_tag_work_hunting::hunting_steps::FIND_TARGET : find_target(e, h, t, pos); break;
			case ai_tag_work_hunting::hunting_steps::HUNT: hunt(e, h, t, pos); break;
			}
		}

		void run(const double &duration_ms) {
			work.do_work(jobs_board::evaluate_hunting, dispatch, job_tag);
		}
	}
}
