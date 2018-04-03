#include "stdafx.h"
#include "jobs_board.hpp"
#include "ai_work_template.hpp"
#include "../../../global_assets/game_designations.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../helpers/targeted_flow_map.hpp"
#include "../../physics/trigger_system.hpp"

namespace systems {
	namespace ai_lever {

		using namespace bengine;
		using namespace jobs_board;
		using namespace dijkstra;
		using namespace distance_map;

		namespace jobs_board {
			void evaluate_levers(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (designations->levers_to_pull.empty()) return; // Nothing to cut down

				board.insert(std::make_pair(1, jt)); // Lever pulling is a TOP priority!
			}
		}

		static bool first_run = true;

		void run(const double &duration_ms) {
			if (first_run) {
				first_run = false;
				register_job_offer<ai_tag_work_pull_lever>(jobs_board::evaluate_levers);
			}

			ai_work_template<ai_tag_work_pull_lever> work;
			work.do_ai("Lever Operation", [&work](entity_t &e, ai_tag_work_pull_lever &l, ai_tag_my_turn_t &t, position_t &pos) {
				if (l.step == ai_tag_work_pull_lever::lever_steps::FIND) {
					if (!l.current_path)
					{
						if (designations->levers_to_pull.empty())
						{
							work.cancel_work_tag(e);
							return;
						}
						const auto lever_id = designations->levers_to_pull[0];
						const auto lever = entity(lever_id);
						if (!lever)
						{
							work.cancel_work_tag(e);
							return;
						}
						const auto lpos = lever->component<position_t>();
						if (!lpos)
						{
							work.cancel_work_tag(e);
							return;
						}
						l.current_path = find_path(pos, *lpos);
						if (!l.current_path->success)
						{
							work.cancel_work_tag(e);
							return;
						}

						designations->levers_to_pull.erase(
							std::remove_if(designations->levers_to_pull.begin(), designations->levers_to_pull.end(), [&lever_id](auto l) { return l == lever_id; }),
							designations->levers_to_pull.end()
						);
					}

					// Path towards the harvest
					work.follow_path(l, pos, e, [&e, &work]() {
						// On cancel
						work.cancel_work_tag(e);
						return;
					}, [&e, &pos, &l, &work] {
						// On success
						l.step = ai_tag_work_pull_lever::lever_steps::PULL;
					});
					return;
				}
				else if (l.step == ai_tag_work_pull_lever::lever_steps::PULL) {
					auto lever_id = 0;
					each<lever_t, position_t>([&lever_id, &pos](entity_t &E, lever_t &L, position_t &p) {
						if (p == pos) lever_id = E.id;
					});
					if (lever_id == 0) throw std::runtime_error("Unidentified lever!");

					// Remove from designations
					designations->levers_to_pull.erase(std::remove_if(
						designations->levers_to_pull.begin(),
						designations->levers_to_pull.end(),
						[&lever_id](auto n) { return n == lever_id; }
					), designations->levers_to_pull.end());

					// Actually pull the lever
					// TODO: emit_deferred(lever_pulled_message{ lever_id });
					systems::triggers::lever_pulled.enqueue(triggers::lever_pulled_message{ lever_id });

					// Idle
					work.cancel_work_tag(e);
					call_home("AI", "Lever Pull");
					return;
				}
			});
		}
	}
}
