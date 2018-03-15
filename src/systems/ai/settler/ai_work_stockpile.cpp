#include "ai_work_stockpile.hpp"
#include "../stockpile_system.hpp"
#include "jobs_board.hpp"
#include "templated_work_steps_t.hpp"

namespace systems {
	namespace ai_work_stockpiles {
		using namespace bengine;
		using namespace jobs_board;

		namespace jobs_board {
			void evaluate_stockpile_tidying(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (stockpile_system::storable_items.empty()) return;
				board.insert(std::make_pair(30, jt));
			}
		}

		static const char * job_tag = "Stockpile Maintenance";
		static work::templated_work_steps_t<ai_tag_work_stockpiles_t> work;


		inline void find_item(entity_t &e, ai_tag_work_stockpiles_t &h, ai_tag_my_turn_t &t, position_t &pos) {
			for (auto &sp : stockpile_system::storable_items)
			{
				if (!sp.deleteme) {
					const auto loc = inventory::get_item_location(sp.item_id);
					h.current_path = find_path(pos, *loc);
					if (h.current_path->success)
					{
						h.tool_id = sp.item_id;
						const auto stockpile_id = sp.dest_tile;
						if (stockpile_system::stockpiles[stockpile_id].free_capacity > 0) {
							--stockpile_system::stockpiles[stockpile_id].free_capacity;
							h.destination = *stockpile_system::stockpiles[stockpile_id].open_tiles.begin();
							stockpile_system::stockpiles[stockpile_id].open_tiles.erase(h.destination);
							h.step = ai_tag_work_stockpiles_t::GOTO_ITEM;
							sp.deleteme = true;
							goto cleanup;
						}
					}
				}
			}
			work.cancel_work_tag(e);

			cleanup:
			stockpile_system::storable_items.erase(
				std::remove_if(stockpile_system::storable_items.begin(),
					stockpile_system::storable_items.end(),
					[](const auto &si) { return si.deleteme; }),
				stockpile_system::storable_items.end()
			);
		}

		inline void goto_item(entity_t &e, ai_tag_work_stockpiles_t &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_stockpiles_t::FIND_ITEM;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_stockpiles_t::GOTO_STOCKPILE;

				inventory_system::pickup_item(h.tool_id, e.id);
				return;
			});
		}

		inline void goto_stockpile(entity_t &e, ai_tag_work_stockpiles_t &h, ai_tag_my_turn_t &t, position_t &pos) {
			if (!h.current_path)
			{
				auto[x, y, z] = idxmap(h.destination);
				h.current_path = find_path(pos, position_t{ x, y, z });
				if (!h.current_path->success)
				{
					inventory_system::drop_item(h.tool_id, pos.x, pos.y, pos.z);
					work.cancel_work_tag(e);
					return;
				}
			}

			work.follow_path(h, pos, e, [&h, &pos, &e]() {
				// Cancel
				inventory_system::drop_item(h.tool_id, pos.x, pos.y, pos.z);
				work.cancel_work_tag(e);
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_stockpiles_t::DROP_ITEM;
				return;
			});
		}

		inline void drop_item(entity_t &e, ai_tag_work_stockpiles_t &h, ai_tag_my_turn_t &t, position_t &pos) {
			inventory_system::drop_item(h.tool_id, pos.x, pos.y, pos.z);
			work.cancel_work_tag(e);
		}

		void dispatch(entity_t &e, ai_tag_work_stockpiles_t &h, ai_tag_my_turn_t &t, position_t &pos) {
			switch (h.step)
			{
			case ai_tag_work_stockpiles_t::FIND_ITEM : find_item(e, h, t, pos); break;
			case ai_tag_work_stockpiles_t::GOTO_ITEM: goto_item(e, h, t, pos); break;
			case ai_tag_work_stockpiles_t::GOTO_STOCKPILE: goto_stockpile(e, h, t, pos); break;
			case ai_tag_work_stockpiles_t::DROP_ITEM: drop_item(e, h, t, pos); break;
			}
		}

		void run(const double &duration_ms)
		{
			work.do_work(jobs_board::evaluate_stockpile_tidying, dispatch, job_tag);
		}
	}
}
