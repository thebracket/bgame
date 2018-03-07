#pragma once

#include "jobs_board.hpp"
#include "../../../bengine/ecs.hpp"
#include "../../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../../components/position.hpp"
#include "../../../components/settler_ai.hpp"
#include "../../../components/ai_tags/ai_mode_idle.hpp"
#include "../../../components/claimed_t.hpp"
#include "../../../components/game_stats.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../helpers/pathfinding.hpp"
#include "../../../global_assets/rng.hpp"
#include "../../damage/damage_system.hpp"
#include "../../../planet/region/region.hpp"
#include "../../physics/movement_system.hpp"
#include "../../../global_assets/game_ecs.hpp"

namespace work {
	
	template <typename WORK_TAG>
	struct templated_work_steps_t {

		template <typename REGISTER, typename DISPATCH>
		void do_work(const REGISTER &&registration, const DISPATCH &dispatch, const char * new_status) 
		{
			using namespace bengine;

			// Handle jobs board registration
			if (first_run) {
				jobs_board::register_job_offer<WORK_TAG>(registration);
				first_run = false;
			}

			// Perform tag dispatch
			each<WORK_TAG, ai_tag_my_turn_t, position_t, settler_ai_t>([&new_status, &dispatch] (entity_t &e, WORK_TAG &tag, ai_tag_my_turn_t &turn, position_t &pos, settler_ai_t &ai) {
				// It's not my turn anymore - so remove the component
				bengine::delete_component<ai_tag_my_turn_t>(e.id);

				// Set the status
				ai.job_status = std::string(new_status);

				// Dispatch to find the step handler
				dispatch(e, tag, turn, pos);

				// If not tagged for this work type, go idle
				if (e.component<WORK_TAG>() == nullptr) {
					e.assign(ai_mode_idle_t{});
				}
			});
		}

		static void set_status(bengine::entity_t &e, const char * status) {
			auto ai = e.component<settler_ai_t>();
			if (ai) {
				ai->job_status = std::string(status);
			}
		}

		static void cancel_work_tag(bengine::entity_t &e) {
			bengine::delete_component<WORK_TAG>(e.id);
			set_status(e, "Idle");
		}

		void move_to(const int &entity_id, const position_t &start, const position_t &destination) const {
			systems::movement::move_to(*bengine::entity(entity_id), start, destination);
			//emit_deferred(entity_wants_to_move_message{ entity_id, destination });
		}

		template <typename CANCEL, typename ARRIVED>
		void follow_path(WORK_TAG &tag, position_t &pos, bengine::entity_t &e, const CANCEL &&cancel, const ARRIVED &&arrived) {
			if (!tag.current_path || !tag.current_path->success) {
				cancel();
				return;
			}
			if (pos == tag.current_path->destination || tag.current_path->steps.empty()) {
				arrived();
				return;
			}

			const position_t next_step = tag.current_path->steps.front();
			tag.current_path->steps.pop_front();
			if (next_step.x > 0 && next_step.x < REGION_WIDTH && next_step.y > 0 &&
				next_step.y < REGION_HEIGHT && next_step.z > 0 && next_step.z < REGION_DEPTH
				&& region::flag(mapidx(next_step), tile_flags::CAN_STAND_HERE))
			{
				move_to(e.id, pos, next_step);
			}
			else {
				// We couldn't get there
				tag.current_path.reset();
				cancel();
			}
		}

	private:
		bool first_run = true;
	};

	template <typename ITEM_TYPE>
	inline bool do_i_have_tool(bengine::entity_t &e) {
		using namespace bengine;

		bool have_tool = false;
		each<item_t, item_carried_t, claimed_t, ITEM_TYPE>([&e, &have_tool](entity_t &E, item_t &i, item_carried_t &ic, claimed_t &claimed, ITEM_TYPE &tool) {
			if (ic.carried_by == e.id) have_tool = true;
		});
		return have_tool;
	}

	template <typename ITEM_TYPE, typename JOB_TYPE>
	std::size_t find_claimed_item(bengine::entity_t &e) {
		using namespace bengine;

		std::size_t tool_id = 0;
		each<item_t, claimed_t, ITEM_TYPE>([&e, &tool_id](entity_t &E, item_t &i, claimed_t &claimed, ITEM_TYPE &tool) {
			if (claimed.claimed_by == e.id) tool_id = E.id;
		});
		if (tool_id > 0) {
			return tool_id;
		}

		// We couldn't find a tool allocated to the poor laborer, so we'll try to find a free one and claim it
		each_without<claimed_t, item_t, ITEM_TYPE>([&e, &tool_id](entity_t &E, item_t &i, ITEM_TYPE &tool) {
			E.assign(claimed_t{ e.id });
			tool_id = E.id;
		});
		if (tool_id > 0) {
			return tool_id;
		}

		// We still couldn't find one. It isn't possible to be a JOB_TYPE anymore!
		delete_component<JOB_TYPE>(e.id);
		auto stats = e.component<game_stats_t>();
		if (stats) {
			stats->profession_tag = stats->original_profession;
			// TODO: Unhappy thoughts on firing
		}
		return 0;
	}

	template <typename ITEM_TYPE, typename JOB_TYPE, typename WORK_TAG, typename I_HAVE_IT_ALREADY, typename I_NEED_TO_GO_THERE>
	inline void get_tool(bengine::entity_t &e, templated_work_steps_t<WORK_TAG> &work, WORK_TAG &job, 
		const position_t &pos, const I_HAVE_IT_ALREADY & on_already_have_it, const I_NEED_TO_GO_THERE & on_need_to_travel) 
	{
		// Do I already have the tool?
		if (do_i_have_tool<ITEM_TYPE>(e)) {
			// We already have the tool, proceed to next step
			on_already_have_it();
			return;
		}

		// We need to find the tool if we made it this far
		auto tool_id = find_claimed_item<ITEM_TYPE, JOB_TYPE>(e);
		if (tool_id == 0) {
			// We couldn't be a JOB_TYPE anymore, no tools!
			work.cancel_work_tag(e);
			return;
		}
		job.tool_id = tool_id;

		// Find the tool and build a path to it
		auto tool_pos = inventory::get_item_location(job.tool_id);
		if (!tool_pos) {
			// We couldn't get there - cancel
			work.cancel_work_tag(e);
			return;
		}
		job.current_path.reset();
		job.current_path = find_path(pos, *tool_pos);
		if (!job.current_path->success) {
			// We couldn't get there - cancel
			work.cancel_work_tag(e);
			return;
		}
		on_need_to_travel();
	}

	template <typename WORK_TAG>
	inline bool skill_check_or_damage(bengine::entity_t &e, const char * skill_name, const int &difficulty, templated_work_steps_t<WORK_TAG> &work, const int damage, const char * damage_message) {
		auto stats = e.component<game_stats_t>();
		if (!stats) {
			work.cancel_work_tag(e);
			return false;
		}

		auto skill_check = skill_roll(e.id, *stats, rng, skill_name, difficulty);
		if (skill_check >= SUCCESS) {
			return true;
		}
		else if (skill_check == CRITICAL_FAIL) {
			systems::damage_system::inflict_damage(systems::damage_system::inflict_damage_message{ e.id, damage, damage_message });
		}
		return false;
	}
}
