#include "stdafx.h"
#include "jobs_board.hpp"
#include "../../../global_assets/game_designations.hpp"
#include "../../helpers/weapons_helper.hpp"
#include "../../../bengine/geometry.hpp"
#include "ai_work_template.hpp"

namespace systems {
	namespace ai_guard {

		using namespace bengine;
		using namespace jobs_board;
		using namespace weapons;

		namespace jobs_board {
			void evaluate_guarding(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (designations->guard_points.empty()) return; // Nothing to guard
				if (shooting_range(e, pos)<1) return; // No gun
				for (const auto &g : designations->guard_points) {
					if (!g.first) {
						board[static_cast<int>(distance3d(pos.x, pos.y, pos.z, g.second.x, g.second.y, g.second.z))] = jt;
					}
				}
			}
		}

		static bool first_run = true;

		void run(const double &duration_ms) {
			if (first_run) {
				first_run = false;
				register_job_offer<ai_tag_work_guarding>(jobs_board::evaluate_guarding);
			}

			ai_work_template<ai_tag_work_guarding> work;

			work.do_ai("Guard Duty", [&work](entity_t &e, ai_tag_work_guarding &g, ai_tag_my_turn_t &t, position_t &pos) {
				if (g.step == ai_tag_work_guarding::GOTO_POST) {
					if (!g.has_post) {
						//std::cout << "Selecting a post\n";
						// Select post and claim it
						float distance = std::numeric_limits<float>().max();
						std::size_t i = 0;
						std::size_t idx = std::numeric_limits<std::size_t>().max();
						for (auto &gp : designations->guard_points) {
							if (!gp.first) {
								float d = distance3d(pos.x, pos.y, pos.z, gp.second.x, gp.second.y, gp.second.z);
								if (d < distance) {
									distance = d;
									idx = i;
								}
								else {
									//std::cout << "Guardpost already claimed\n";
								}
							}
							++i;
						}

						//std::cout << "Picked: " << idx << "\n";

						if (idx == std::numeric_limits<std::size_t>().max()) {
							work.cancel_work_tag(e);
						}
						else {
							g.guard_post = designations->guard_points[idx].second;
							designations->guard_points[idx].first = true;
							g.has_post = true;
						}
						return;
					}

					if (!g.current_path) {
						// Determine a path - bail out if none
						g.current_path = find_path(pos, g.guard_post);
						if (!g.current_path->success) {
							work.cancel_work_tag(e);
							for (auto &gp : designations->guard_points) {
								if (gp.second == g.guard_post) gp.first = false;
							}
						}
						//std::cout << "Found a path\n";
						return;
					}
					else {
						work.follow_path(g, pos, e, [&g, &e, &work]() {
							// Cancel
							g.current_path.reset();
							work.cancel_work_tag(e);
							for (auto &gp : designations->guard_points) {
								if (gp.second == g.guard_post) gp.first = false;
							}
							return;
						}, [&g]() {
							// Arrived
							g.current_path.reset();
							g.step = ai_tag_work_guarding::guard_steps::GUARD;
							return;
						});
					}
				}
				else if (g.step == ai_tag_work_guarding::guard_steps::GUARD) {
					// Do we still have ammo? If not, stop guarding.
					if (shooting_range(e, pos) == 0) {
						work.cancel_work_tag(e);
						for (auto &gp : designations->guard_points) {
							if (gp.second == g.guard_post) gp.first = false;
						}
					}
				}
			});
		}
	}
}