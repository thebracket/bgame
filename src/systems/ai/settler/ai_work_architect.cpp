#include "stdafx.h"
#include "ai_work_architect.hpp"
#include "templated_work_steps_t.hpp"
#include "../../../global_assets/architecture_designations.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "../mining_system.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../architecture_system.hpp"
#include "../distance_map_system.hpp"
#include "../../helpers/targeted_flow_map.hpp"
#include "../../../render_engine/vox/renderables.hpp"
#include "../../physics/movement_system.hpp"

using namespace tile_flags;

namespace systems {
	namespace ai_architect {

		using namespace bengine;
		using namespace jobs_board;
		using namespace movement;
		using namespace region;

		namespace jobs_board {
			void evaluate_architecture(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (architecture_designations->architecture.empty()) return;
				if (inventory::blocks_available() == 0) return; // No blocks
				board.insert(std::make_pair(5, jt));
			}
		}

		static const char * job_tag = "Construction";
		static work::templated_work_steps_t<ai_tag_work_architect> work;

		inline void goto_block(entity_t &e, ai_tag_work_architect &a, ai_tag_my_turn_t &t, position_t &pos) {
			if (!a.current_path) {
				// Find the closest block
				std::map<int, size_t> block_distances;
				each_without<claimed_t, item_t>([&pos, &block_distances](entity_t &item_entity, item_t &item)
				{
					if (item.item_tag == "block") {
						auto item_loc = inventory::get_item_location(item_entity.id);
						if (item_loc) {
							block_distances.insert(std::make_pair(static_cast<int>(distance3d(pos.x, pos.y, pos.z, item_loc->x, item_loc->y, item_loc->z)), item_entity.id));
						}
					}
				});

				// Claim it
				if (block_distances.empty())
				{
					work.cancel_work_tag(e);
					return;
				}
				const auto item_entity = entity(block_distances.begin()->second);
				if (!item_entity)
				{
					work.cancel_work_tag(e);
					return;
				}
				item_entity->assign(claimed_t{ e.id });
				a.current_tool = item_entity->id;

				// Path to it
				a.current_path = find_path(pos, *inventory::get_item_location(a.current_tool));
				if (!a.current_path || !a.current_path->success)
				{
					a.current_path.reset();
					work.cancel_work_tag(e);
					inventory_system::claim_item(a.current_tool, false);
					return;
				}
			}

			if (entity(a.current_tool) == nullptr)
			{
				//std::cout << "WARNING: Item became null!\n";
				work.cancel_work_tag(e);
			}

			work.follow_path(a, pos, e, [&e, &pos, &a]()
			{
				// Cancel
				a.current_path.reset();
				if (a.current_tool > 0) {
					inventory_system::drop_item(a.current_tool, pos.x, pos.y, pos.z);
					inventory_system::claim_item(a.current_tool, false);
				}
				work.cancel_work_tag(e);
			}, [&a]()
			{
				// Success
				a.step = ai_tag_work_architect::architect_steps::COLLECT_BLOCK;
				a.current_path.reset();
			});
		}

		inline void collect_block(entity_t &e, ai_tag_work_architect &a, ai_tag_my_turn_t &t, position_t &pos) {
			if (a.current_tool == 0)
			{
				//std::cout << "Warning: no block selected\n";
			}
			a.current_path.reset();
			inventory_system::pickup_item(a.current_tool, e.id);
			//inventory_system::claim_item(a.current_tool, false);
			a.step = ai_tag_work_architect::architect_steps::GOTO_SITE;
		}

		inline void goto_site(entity_t &e, ai_tag_work_architect &a, ai_tag_my_turn_t &t, position_t &pos) {
			using namespace architecture_system;

			if (a.current_path)
			{
				// We have a path - follow it to the target
				work.follow_path(a, pos, e, [&e, &pos, &a]()
				{
					// Cancel
					a.current_path.reset();
				}, [&a]()
				{
					// Success
					a.step = ai_tag_work_architect::architect_steps::BUILD;
					a.current_path.reset();
				});
			} else
			{
				// Try to find a path to work
				if (architecture_designations->architecture.empty())
				{
					inventory_system::drop_item(a.current_tool, pos.x, pos.y, pos.z);
					inventory_system::claim_item(a.current_tool, false);
					work.cancel_work_tag(e);
					return;
				}
				const auto finder = architecture_map->find_nearest_reachable_target(pos);
				if (finder.target == 0)
				{
					// Nothing to do
					inventory_system::drop_item(a.current_tool, pos.x, pos.y, pos.z);
					inventory_system::claim_item(a.current_tool, false);
					work.cancel_work_tag(e);
					return;
				} else
				{
					a.current_path = finder.path;
					a.target_tile = finder.target;
				}
			}
		}

		inline void build(entity_t &e, ai_tag_work_architect &a, ai_tag_my_turn_t &t, position_t &pos) {
			using namespace architecture_system;
			const auto idx = mapidx(pos);
			const auto bidx = a.target_tile;

			if (!architecture_map->is_target(idx))
			{
				// Nothing to do
				a.current_path.reset();
				inventory_system::drop_item(a.current_tool, pos.x, pos.y, pos.z);
				inventory_system::claim_item(a.current_tool, false);
				work.cancel_work_tag(e);
				return;
			}

			const auto finder = architecture_designations->architecture.find(bidx);
			if (finder != architecture_designations->architecture.end()) {
				const auto build_type = finder->second;

				std::size_t material = 0;
				auto block_e = entity(a.current_tool);
				if (block_e) {
					const auto the_item = block_e->component<item_t>();
					if (the_item) {
						material = the_item->material;
					}
					else
					{
						//std::cout << "Warning: Item does not exist\n";
						work.cancel_work_tag(e);
						inventory_system::drop_item(a.current_tool, pos.x, pos.y, pos.z);
						inventory_system::claim_item(a.current_tool, false);
					}
				}
				if (material == 0)
				{
					//std::cout << "Warning: material is invalid\n";
				}
				inventory_system::destroy_item(a.current_tool);
				a.current_tool = 0;

				set_flag(bidx, CONSTRUCTION);
				if (material > 0) set_tile_material(bidx, material);

				if (build_type == 0) {
					// Wall
					make_wall(bidx, material);
					set_flag(bidx, CONSTRUCTION);
					call_home("AI", "Architecture", "Wall");
				}
				else if (build_type == 1) {
					// Floor
					make_floor(bidx, material);
					call_home("AI", "Architecture", "Floor");
					set_flag(bidx, CONSTRUCTION);
					region::reset_flag(bidx, CONSTRUCTION); // Mark it as a constructed floor
				}
				else if (build_type == 2) {
					// Up
					make_stairs_up(bidx, material);
					set_flag(bidx, CONSTRUCTION);
					call_home("AI", "Architecture", "Up");
				}
				else if (build_type == 3) {
					// Down
					make_stairs_down(bidx, material);
					set_flag(bidx, CONSTRUCTION);
					call_home("AI", "Architecture", "Down");
				}
				else if (build_type == 4) {
					// UpDown
					make_stairs_updown(bidx, material);
					set_flag(bidx, CONSTRUCTION);
					call_home("AI", "Architecture", "UpDown");
				}
				else if (build_type == 5) {
					// Ramp
					make_ramp(bidx, material);
					set_flag(bidx, CONSTRUCTION);
					call_home("AI", "Architecture", "Ramp");
				}
				else if (build_type == 6) {
					make_floor(bidx, material);
					set_flag(bidx, CONSTRUCTION);

					// We need to iterate through the bridge tiles and see if it is done yet.
					const auto bid = bridge_id(bidx);
					auto complete = true;
					auto bridge_idx = 0;
					each_bridge([&bridge_idx, &bid, &bidx, &complete](std::size_t id) {
						if (id == bid && bridge_idx != bidx && architecture_designations->architecture.find(bridge_idx) != architecture_designations->architecture.end()) {
							complete = false;
						}
						++bridge_idx;
					});
					if (complete) {
						entity(bid)->component<bridge_t>()->complete = true;
						entity(bid)->assign(receives_signal_t{});
						entity(bid)->assign(name_t{ "Bridge", "" });
						auto[bx, by, bz] = idxmap(bidx);
						entity(bid)->assign(position_t{ bx, by, bz });
					}
					call_home("AI", "Architecture", "Bridge");
				}

				auto[cx, cy, cz] = idxmap(bidx);
				tile_calculate(cx, cy, cz);
				for (int Z = -2; Z<3; ++Z) {
					for (int Y = -2; Y<3; ++Y) {
						for (int X = -2; X<3; ++X) {
							tile_calculate(cx + X, cy + Y, cz + Z);
						}
					}
				}
				chunks::mark_chunk_dirty_by_tileidx(bidx);

				architecture_designations->architecture.erase(bidx);
				mining_system::mining_map_changed();
				architecture_map_changed();
				distance_map::refresh_all_distance_maps();
				render::models_changed = true;
				// TODO: emit(map_changed_message{});
				call_home("architecture", std::to_string(build_type));
			}
			else {
				inventory_system::drop_item(a.current_tool, pos.x, pos.y, pos.z);
				inventory_system::claim_item(a.current_tool, false);
				work.cancel_work_tag(e);
				return;
			}
			work.cancel_work_tag(e);
		}

		void dispatch(entity_t &e, ai_tag_work_architect &a, ai_tag_my_turn_t &t, position_t &pos) {
			switch (a.step)
			{
			case ai_tag_work_architect::architect_steps::GOTO_BLOCK : goto_block(e, a, t, pos); break;
			case ai_tag_work_architect::architect_steps::COLLECT_BLOCK: collect_block(e, a, t, pos); break;
			case ai_tag_work_architect::architect_steps::GOTO_SITE: goto_site(e, a, t, pos); break;
			case ai_tag_work_architect::architect_steps::BUILD: build(e, a, t, pos); break;
			}
		}

		void run(const double &duration_ms) {
			work.do_work(jobs_board::evaluate_architecture, dispatch, job_tag);
		}
	}
}
