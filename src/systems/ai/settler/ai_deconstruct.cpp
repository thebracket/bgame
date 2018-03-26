#include "stdafx.h"
#include "ai_deconstruct.hpp"
#include "templated_work_steps_t.hpp"
#include "jobs_board.hpp"
#include "../../../global_assets/game_designations.hpp"
#include "../../../raws/raws.hpp"

namespace systems {
	namespace ai_deconstruction {
		using namespace bengine;
		using namespace jobs_board;

		namespace jobs_board {

			void evaluate_hunting(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (designations->deconstructions.empty()) return; // Nothing to do

				board.insert(std::make_pair(20, jt));
			}
		}

		static const char * job_tag = "Deconstruction";
		static work::templated_work_steps_t<ai_tag_work_deconstruct> work;

		inline void find_target(entity_t &e, ai_tag_work_deconstruct &a, ai_tag_my_turn_t &t, position_t &pos) {
			if (designations->deconstructions.empty())
			{
				work.cancel_work_tag(e);
				return;
			}
			a.target_building = designations->deconstructions.begin()->building_id;
			designations->deconstructions.erase(
				std::remove_if(designations->deconstructions.begin(), designations->deconstructions.end(), [&a](auto &d) { return d.building_id == a.target_building; }),
				designations->deconstructions.end()
			);
			const auto bentity = entity(a.target_building);
			if (!bentity)
			{
				work.cancel_work_tag(e);
				return;
			}
			const auto position = bentity->component<position_t>();
			if (!position)
			{
				work.cancel_work_tag(e);
				return;
			}
			a.current_path = find_path(pos, *position, true);
			if (!a.current_path->success)
			{
				work.cancel_work_tag(e);
				return;
			}
			a.step = ai_tag_work_deconstruct::deconstruct_steps::GOTO_TARGET;
		}

		inline void goto_target(entity_t &e, ai_tag_work_deconstruct &a, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(a, pos, e, [&e, &a]()
			{
				// Cancel
				a.current_path.reset();
				work.cancel_work_tag(e);
			}, [&a, &e]()
			{
				// Success
				a.current_path.reset();
				a.step = ai_tag_work_deconstruct::deconstruct_steps::UNBUILD;
			});
		}

		inline void unbuild(entity_t &e, ai_tag_work_deconstruct &a, ai_tag_my_turn_t &t, position_t &pos) {
			const auto buiding_entity = entity(a.target_building);
			if (!buiding_entity)
			{
				work.cancel_work_tag(e);
				return;
			}
			const auto building = buiding_entity->component<building_t>();
			if (building)
			{
				// We need to desconstruct it
				for (const auto &component : building->built_with)
				{
					// tag, material
					spawn_item_on_ground(pos.x, pos.y, pos.z, component.first, component.second);
				}
				delete_entity(a.target_building);
			}
		}

		void dispatch(entity_t &e, ai_tag_work_deconstruct &h, ai_tag_my_turn_t &t, position_t &pos) {
			switch (h.step)
			{
			case ai_tag_work_deconstruct::deconstruct_steps::FIND_TARGET: find_target(e, h, t, pos); break;
			case ai_tag_work_deconstruct::deconstruct_steps::GOTO_TARGET: goto_target(e, h, t, pos); break;
			case ai_tag_work_deconstruct::deconstruct_steps::UNBUILD: unbuild(e, h, t, pos); break;
			}
		}

		void run(const double &duration_ms)
		{
			work.do_work(jobs_board::evaluate_hunting, dispatch, job_tag);
		}
	}
}
