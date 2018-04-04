#include "stdafx.h"
#include "jobs_board.hpp"
#include "ai_work_template.hpp"
#include "../../helpers/workflow_assistant.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../../raws/reactions.hpp"
#include "../../../raws/defs/reaction_t.hpp"
#include "../../gui/particle_system.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../../global_assets/building_designations.hpp"
#include "../../../render_engine/vox/renderables.hpp"

namespace systems {
	namespace ai_workorder {

		using namespace bengine;
		using namespace jobs_board;
		using namespace workflow;
		using namespace inventory;

		namespace jobs_board {
			void evaluate_work_orders(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				// Check for available queued reactions
				if (!building_designations->build_orders.empty()) {
					board.insert(std::make_pair(10, jt));
					return;
				}

				// Check for available automatic reactions
				auto ai = e.component<settler_ai_t>();
				if (ai == nullptr) {
					return;
				}
				if (is_auto_reaction_task_available(e.id, *ai)) {
					board.insert(std::make_pair(20, jt));
					return;
				}
			}
		}

		bool first_run = true;

		void run(const double &duration_ms) {
			if (first_run) {
				register_job_offer<ai_tag_work_order>(jobs_board::evaluate_work_orders);
				first_run = false;
			}

			ai_work_template<ai_tag_work_order> work;
			work.do_ai("Fulfilling Work Orders", [&work](entity_t &e, ai_tag_work_order &w, ai_tag_my_turn_t &t, position_t &pos) -> void {
				auto reaction_info = get_reaction_def(w.reaction_target.reaction_tag);
				if (reaction_info) {
					std::string status = reaction_info->name;
					work.set_status(e, std::move(status));
				}
				else {
					work.set_status(e, "Fulfilling Work Orders");
				}
				if (w.step == ai_tag_work_order::work_steps::FIND_JOB) {
					std::unique_ptr<reaction_task_t> autojob;

					if (!building_designations->build_orders.empty()) {
						autojob = find_queued_reaction_task(e.id, w);
					}
					if (!autojob) {
						autojob = find_automatic_reaction_task(e.id, w);
						// TODO: Remove from work queue
						bool found = false;
						for (auto &j : building_designations->build_orders)
						{
							if (j.second == w.reaction_target.reaction_tag && j.first > 1)
							{
								found = true;
								--j.first;
							}
						}
						if (!found)
						{
							building_designations->build_orders.erase(
								std::remove_if(
									building_designations->build_orders.begin(),
									building_designations->build_orders.end(),
									[&w](const auto &b) { return w.reaction_target.reaction_tag == b.second; }
								),
								building_designations->build_orders.end()
							);
						}						
					}

					if (!autojob) {
						work.cancel_work_tag(e);
					}
					else {
						w.reaction_target = *autojob;
						w.step = ai_tag_work_order::work_steps::SELECT_INPUT;
					}
					return;

				}
				else if (w.step == ai_tag_work_order::work_steps::SELECT_INPUT) {
					// If there are no inputs, go to the workshop
					auto reactor_pos = entity(w.reaction_target.building_id)->component<position_t>();
					if (w.reaction_target.components.empty() && !(pos == *reactor_pos)) {
						w.step = ai_tag_work_order::work_steps::GO_TO_WORKSHOP;
						w.current_path = find_path(pos, position_t{ reactor_pos->x, reactor_pos->y, reactor_pos->z });
						return;
					}

					auto has_components = true;
					for (auto &component : w.reaction_target.components) {
						if (!component.second) {
							has_components = false;
							w.current_tool = component.first;
							auto item_loc = get_item_location(w.current_tool);
							if (!item_loc) {
								work.cancel_work_tag(e);
								delete_component<claimed_t>(w.reaction_target.building_id); // Un-claim the workshop
								for (const auto &item : w.reaction_target.components)
								{
									delete_component<claimed_t>(item.first);
								}
								return;
							}
							w.current_path = find_path(pos, *item_loc);
							if (w.current_path->success) {
								component.second = true;
								w.step = ai_tag_work_order::work_steps::GO_TO_INPUT;
							}
							else {
								work.cancel_work_tag(e);
								delete_component<claimed_t>(w.reaction_target.building_id); // Un-claim the workshop
								for (const auto &item : w.reaction_target.components)
								{
									delete_component<claimed_t>(item.first);
								}
							}
							return;
						}
					}

					if (has_components) {
						w.step = ai_tag_work_order::work_steps::REACT;
					}
					return;
				}
				else if (w.step == ai_tag_work_order::work_steps::GO_TO_INPUT) {
					work.follow_path(w, pos, e, [&w, &e, &work]() {
						// Cancel
						unclaim_by_id(w.current_tool);
						work.cancel_work_tag(e);
						delete_component<claimed_t>(w.reaction_target.building_id); // Unclaim the workshop
						for (const auto &item : w.reaction_target.components)
						{
							delete_component<claimed_t>(item.first);
						}
					}, [&w, &e]() {
						// Arrived
						w.current_path.reset();
						w.step = ai_tag_work_order::work_steps::COLLECT_INPUT;
					});
					return;
				}
				else if (w.step == ai_tag_work_order::work_steps::COLLECT_INPUT) {
					// Find the component, remove any position or stored components, add a carried_by component
					inventory_system::pickup_item(w.current_tool, e.id );

					w.step = ai_tag_work_order::work_steps::GO_TO_WORKSHOP;
					const auto reactor_e = entity(w.reaction_target.building_id);
					if (!reactor_e)
					{
						unclaim_by_id(w.current_tool);
						work.cancel_work_tag(e);
						delete_component<claimed_t>(w.reaction_target.building_id); // Unclaim the workshop
						for (const auto &item : w.reaction_target.components)
						{
							delete_component<claimed_t>(item.first);
						}
						return;
					}
					const auto reactor_pos = reactor_e->component<position_t>();
					w.current_path = find_path(pos, position_t{ reactor_pos->x, reactor_pos->y, reactor_pos->z });
					return;
				}
				else if (w.step == ai_tag_work_order::work_steps::GO_TO_WORKSHOP) {
					work.follow_path(w, pos, e, [&w, &e, &pos, &work]() {
						// Cancel
						unclaim_by_id(w.current_tool);
						inventory_system::drop_item(w.current_tool, pos.x, pos.y, pos.z );
						work.cancel_work_tag(e);
						delete_component<claimed_t>(w.reaction_target.building_id); // Unclaim the workshop
						for (const auto &item : w.reaction_target.components)
						{
							delete_component<claimed_t>(item.first);
						}
					}, [&w, &pos, &e]() {
						// Arrived
						w.current_path.reset();
						w.step = ai_tag_work_order::work_steps::DROP_INPUT;
					});
					return;
				}
				else if (w.step == ai_tag_work_order::work_steps::DROP_INPUT) {
					inventory_system::drop_item(w.current_tool, pos.x, pos.y, pos.z );
					w.current_tool = 0;
					w.step = ai_tag_work_order::work_steps::SELECT_INPUT;
					return;
				}
				else if (w.step == ai_tag_work_order::work_steps::REACT) {
					// Skill check, destroy inputs, create outputs
					auto stats = e.component<game_stats_t>();
					auto finder = get_reaction_def(w.reaction_target.reaction_tag);
					auto [skill_check, roll, difference] = skill_roll_ext(e.id, *stats, rng, finder->skill, finder->difficulty);

					if (skill_check >= SUCCESS) {
						// Quality and wear
						uint8_t wear = 100;
						uint8_t quality = 3;

						if (roll == 20) {
							quality = 7; // Masterwork on a natural 20
						}
						else {
							if (difference < 1) {
								quality = 1;
							}
							else if (difference < 2) {
								quality = 2;
							}
							else if (difference < 3) {
								quality = 3;
							}
							else if (difference < 4) {
								quality = 4;
							}
							else if (difference < 5) {
								quality = 5;
							}
							else if (difference < 6) {
								quality = 6;
							}
							else {
								quality = 7;
							}
						}

						// Delete components
						std::size_t material = get_material_by_tag("plasteel");
						std::string mat_names;
						for (auto comp : w.reaction_target.components) {
							if (!entity(comp.first)) {
								work.cancel_work_tag(e);
								return;
							}
							if (!entity(comp.first)->component<item_t>()) {
								work.cancel_work_tag(e);
								return;
							}
							material = entity(comp.first)->component<item_t>()->material;
							mat_names += entity(comp.first)->component<item_t>()->item_name + " ";
							delete_item(comp.first);
						}

						// Spawn results
						std::string cname;
						auto name = e.component<name_t>();
						if (name) cname = name->first_name + std::string(" ") + name->last_name;

						for (auto &output : finder->outputs) {
							for (int i = 0; i < output.second; ++i) {
								bool done = false;

								if (!done && finder->specials.test(special_reaction_cooking)) {
									// This is more complicated, we have to make a special item from the components.
									// The idea is to get something like Roast Asparagus
									auto new_item = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, output.first, material, quality, wear, e.id, cname);
									auto item = new_item->component<item_t>();
									item->item_name = mat_names + item->item_name;
									done = true;
								}

								if (!done && finder->specials.test(special_reaction_tanning)) {
									// This is more complicated, we have to make a special item from the components.
									// The idea is to get something like Roast Asparagus
									auto new_item = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, output.first, material, quality, wear, e.id, cname);
									auto item = new_item->component<item_t>();
									item->item_name = mat_names + item->item_name;
									done = true;
								}

								if (!done) {
									std::string cname_inner;
									auto name_inner = e.component<name_t>();
									if (name_inner) cname_inner = name_inner->first_name + std::string(" ") + name_inner->last_name;
									spawn_item_on_ground(pos.x, pos.y, pos.z, output.first, material, quality, wear, e.id, cname_inner);
									done = true;
								}
							}
						}

						// Emit smoke
						// TODO: if (finder->emits_smoke) emit(emit_particles_message{ 1, pos.x, pos.y, pos.z });

						// Consume power
						// TODO: if (finder->power_drain != 0) emit(power_consumed_message{ finder->power_drain });

						// Finish
						call_home("AI", "Work", w.reaction_target.reaction_tag);
						//free_workshop(w.reaction_target.building_id);
						render::models_changed = true;
						inventory_system::inventory_has_changed();
						particles::block_destruction_effect(pos.x, pos.y, pos.z, 1.0f, 1.0f, 1.0f, particles::PARTICLE_LUMBERJACK);

						// Become idle
						work.cancel_work_tag(e);
					}
				}
			});
		}
	}
}
