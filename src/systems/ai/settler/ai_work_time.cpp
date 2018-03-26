#include "stdafx.h"
#include "ai_work_time.hpp"
#include "jobs_board.hpp"

namespace systems {
	namespace ai_work_time {

		using namespace bengine;
		using namespace jobs_board;

		namespace jobs_board {

			job_board_t job_evaluations(entity_t &e, position_t &pos) {
				job_board_t board;
				evaluate(board, e, pos);

				return board;
			}
		}

		void run(const double &duration_ms) {
			each<settler_ai_t, ai_tag_work_shift_t, position_t>([](entity_t &e, settler_ai_t &ai, ai_tag_work_shift_t &work, position_t &pos) {
				// Do we already have a job? If so, then return to doing it!
				if (is_working(e)) return;

				// Build a job candidates list, goal is to pick the easiest job to complete.
				auto available_jobs = jobs_board::job_evaluations(e, pos);

				if (available_jobs.empty()) {
					// There is nothing to do!
					e.assign(ai_mode_idle_t{});
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
