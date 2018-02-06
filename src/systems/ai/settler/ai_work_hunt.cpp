#include "stdafx.h"
#include "jobs_board.hpp"
#include "ai_work_template.hpp"
#include "../../../components/building.hpp"
#include "../../../components/ai_tags/ai_tag_work_hunting.hpp"

namespace systems {
	namespace ai_hunt {

		using namespace bengine;
		using namespace jobs_board;
		using namespace dijkstra;
		using namespace distance_map;

		namespace jobs_board {
			bool butcher_exist() {
				bool butcher_exists = false;

				each<building_t>([&butcher_exists](entity_t &e, building_t &b) {
					if (b.tag == "butcher" && b.complete == true) butcher_exists = true;
				});

				return butcher_exists;
			}

			void evaluate_hunting(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (!butcher_exist()) return;
				const auto distance = huntables_map.get(mapidx(pos));
				if (distance > MAX_DIJSTRA_DISTANCE - 1) return;
				board.insert(std::make_pair(distance, jt));
			}
		}

		static bool first_run = true;

		void run(const double &duration_ms) {
			if (first_run) {
				first_run = false;
				register_job_offer<ai_tag_work_hunting>(jobs_board::evaluate_hunting);
			}

			ai_work_template<ai_tag_work_hunting> work;

			work.do_ai("Hunting", [&work](entity_t &e, ai_tag_work_hunting &h, ai_tag_my_turn_t &t, position_t &pos) {
				const auto distance = huntables_map.get(mapidx(pos));
				if (distance > MAX_DIJSTRA_DISTANCE - 1) {
					work.cancel_work_tag(e);
					return;
				}

				work.folllow_path(huntables_map, pos, e, [&e, &work]() {
					// Cancel
					work.cancel_work_tag(e);
					return;
				}, [&e, &work]() {
					// At destination - stop hunting
					work.cancel_work_tag(e);
					return;
				});
			});
		}
	}
}