#include "ai_leisure_eat.hpp"
#include "templated_idle_steps_t.hpp"
#include "../../gui/log_system.hpp"

namespace systems {
	namespace ai_leisure_eat {
		using namespace bengine;
		using namespace jobs_board;
		using namespace region;

		namespace jobs_board {
			void evaluate_hunger(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				const auto hunger = e.component<hunger_t>();
				if (!hunger) return;
				if (!hunger->is_hungry) return;
				if (!inventory::is_item_category_available<item_food_prepared_t>()) return;
				board.insert(std::make_pair(2, jt)); // Eating is a high priority!
			}
		}

		static const char * job_tag = "Eating";
		static work::templated_idle_steps_t<ai_tag_leisure_eat_t> work;

		inline void find_food(entity_t &e, ai_tag_leisure_eat_t &h, ai_tag_my_turn_t &t, position_t &pos) {
			h.current_path.reset();
			const auto food_id = inventory::find_closest_unclaimed_item_by_category_and_claim_it_immediately<item_food_prepared_t>(e.id, pos);
			if (food_id > 0) {
				const auto food_pos = inventory::get_item_location(food_id);
				if (!food_pos) {
					inventory::unclaim_by_id(food_id);
					work.cancel_work_tag(e);
				}
				else {
					h.tool_id = food_id;
					h.current_path = find_path(pos, *food_pos);
					if (!h.current_path->success) {
						inventory::unclaim_by_id(food_id);
						work.cancel_work_tag(e);
					}
					else {
						h.step = ai_tag_leisure_eat_t::eat_steps::GOTO_FOOD;
					}
				}
			}
			else {
				work.cancel_work_tag(e);
			}
		}

		inline void goto_food(entity_t &e, ai_tag_leisure_eat_t &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_leisure_eat_t::eat_steps::FIND_FOOD;
				inventory::unclaim_by_id(h.tool_id);
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_leisure_eat_t::eat_steps::EAT_FOOD;

				inventory_system::pickup_item(h.tool_id, e.id);
				return;
			});
		}

		inline void eat_food(entity_t &e, ai_tag_leisure_eat_t &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.cancel_work_tag(e);
			const auto hunger = e.component<hunger_t>();
			if (hunger) {
				hunger->is_hungry = false;
				hunger->hunger_clock = 24 * 60;
			}
			const auto food_entity = entity(h.tool_id);
			if (food_entity) {
				const auto name = food_entity->component<item_t>();
				if (name) {
					logging::log_message lmsg{ LOG{}.settler_name(e.id)->text(std::string(" eats "))->text(name->item_name)->chars };
					logging::log(lmsg);
				}
				inventory::delete_item(h.tool_id);
			}
		}

		void dispatch(entity_t &e, ai_tag_leisure_eat_t &h, ai_tag_my_turn_t &t, position_t &pos) {
			switch (h.step) {
			case ai_tag_leisure_eat_t::eat_steps::FIND_FOOD: find_food(e, h, t, pos); break;
			case ai_tag_leisure_eat_t::eat_steps::GOTO_FOOD: goto_food(e, h, t, pos); break;
			case ai_tag_leisure_eat_t::eat_steps::EAT_FOOD: eat_food(e, h, t, pos); break;
			}
		}

		void run(const double &duration_ms)
		{
			work.do_idle(jobs_board::evaluate_hunger, dispatch, job_tag);
		}
	}
}