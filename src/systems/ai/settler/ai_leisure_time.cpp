#include "stdafx.h"
#include "ai_leisure_time.hpp"
#include "../../physics/movement_system.hpp"
#include "templated_work_steps_t.hpp"
#include "jobs_board.hpp"

namespace systems {
	namespace ai_leisure_time {

		using namespace bengine;
		using namespace jobs_board;

		namespace jobs_board {
			job_board_t idle_evaluations(entity_t &e, position_t &pos) {
				job_board_t board;
				evaluate_idle(board, e, pos);

				return board;
			}
		}

		void run(const double &duration_ms) {
			each<settler_ai_t, ai_tag_leisure_shift_t, position_t>([](entity_t &e, settler_ai_t &ai, ai_tag_leisure_shift_t &leisure, position_t &pos) {
				// Do we already have a leisure job? If so, then return to doing it!
				if (is_working_on_leisure(e)) return;

				// Build a job candidates list, goal is to pick the easiest job to complete.
				auto available_jobs = jobs_board::idle_evaluations(e, pos);

				if (available_jobs.empty()) {
					// Nothing to do here
					e.assign(ai_mode_idle_t{});
					delete_component<ai_tag_leisure_shift_t>(e.id);
					delete_component<ai_tag_my_turn_t>(e.id);
					movement::request_random_move(e.id);
					return;
				}
				else {
					delete_component<ai_tag_work_shift_t>(e.id);
				}

				auto job_type = available_jobs.begin()->second;
				job_type->set_tag(e);								
			});
		}
	}
}
