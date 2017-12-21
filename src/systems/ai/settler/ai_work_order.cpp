#include "stdafx.h"
#include "ai_sleep_time.hpp"
#include "jobs_board.hpp"
#include "ai_work_template.hpp"
#include "../../../global_assets/game_designations.hpp"
#include "../../../components/ai_tags/ai_tag_work_order.hpp"
#include "../../helpers/workflow_assistant.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../../raws/reactions.hpp"
#include "../../../raws/defs/reaction_t.hpp"
#include "../inventory_system.hpp"
#include "../../../render_engine/vox/renderables.hpp"
#include "../../gui/particle_system.hpp"
#include "../../../bengine/telemetry.hpp"

namespace systems {
	namespace ai_workorder {

		using namespace bengine;
		using namespace jobs_board;
		using namespace workflow;
		using namespace inventory;

		namespace jobs_board {
			void evaluate_work_orders(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				// Check for available queued reactions
				if (!designations->build_orders.empty()) {
					board.insert(std::make_pair(10, jt));
					//std::cout << "Offering queued build order\n";
					return;
				}

				// Check for available automatic reactions
				auto ai = e.component<settler_ai_t>();
				if (ai == nullptr) {
					//std::cout << "Warning: bailing on null AI\n";
					return;
				}
				if (is_auto_reaction_task_available(*ai)) {
					board.insert(std::make_pair(20, jt));
					//std::cout << "Offering automatic build order\n";
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
			work.do_ai([&work](entity_t &e, ai_tag_work_order &w, ai_tag_my_turn_t &t, position_t &pos) {
				work.set_status(e, "Fulfilling Work Orders");
				if (w.step == ai_tag_work_order::work_steps::FIND_JOB) {
					//std::cout << "Finding work\n";
					std::unique_ptr<reaction_task_t> autojob;

					if (!designations->build_orders.empty()) {
						autojob = find_queued_reaction_task(w);
						//if (autojob) std::cout << "Queued reaction added\n";
					}
					if (!autojob) {
						autojob = find_automatic_reaction_task(w);
						//if (autojob) std::cout << "Automatic reaction added\n";
					}

					if (!autojob) {
						//std::cout << "Bailing out - no available reaction\n";
						delete_component<ai_tag_work_order>(e.id);
					}
					else {
						//std::cout << "Setting up workflow\n";
						w.reaction_target = *autojob;
						w.step = ai_tag_work_order::work_steps::SELECT_INPUT;
					}
					return;

				}
				else if (w.step == ai_tag_work_order::work_steps::SELECT_INPUT) {
					//std::cout << "Input selection\n";
					// If there are no inputs, go to the workshop
					auto reactor_pos = entity(w.reaction_target.building_id)->component<position_t>();
					if (w.reaction_target.components.empty() && !(pos == *reactor_pos)) {
						w.step = ai_tag_work_order::work_steps::GO_TO_WORKSHOP;
						w.current_path = find_path(pos, position_t{ reactor_pos->x, reactor_pos->y, reactor_pos->z });
						return;
					}

					bool has_components = true;
					for (std::pair<std::size_t, bool> &component : w.reaction_target.components) {
						if (!component.second) {
							has_components = false;
							w.current_tool = component.first;
							auto item_loc = get_item_location(w.current_tool);
							if (!item_loc) {
								//std::cout << "Bailing - invalid item\n";
								delete_component<ai_tag_work_order>(e.id);
								return;
							}
							w.current_path = find_path(pos, *item_loc);
							if (w.current_path->success) {
								component.second = true;
								w.step = ai_tag_work_order::work_steps::GO_TO_INPUT;
							}
							else {
								//std::cout << "Bailing - no path to item\n";
								delete_component<ai_tag_work_order>(e.id);
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
					//std::cout << "Go to input\n";
					work.follow_path(w, pos, e, [&w, &e]() {
						// Cancel
						unclaim_by_id(w.current_tool);
						delete_component<ai_tag_work_order>(e.id);
					}, [&w, &e]() {
						// Arrived
						w.current_path.reset();
						w.step = ai_tag_work_order::work_steps::COLLECT_INPUT;
						//std::cout << "Cancelling - input pathing failed\n";
					});
					return;
				}
				else if (w.step == ai_tag_work_order::work_steps::COLLECT_INPUT) {
					//std::cout << "Collect input\n";
					// Find the component, remove any position or stored components, add a carried_by component
					inventory_system::pickup_item(w.current_tool, e.id );

					w.step = ai_tag_work_order::work_steps::GO_TO_WORKSHOP;
					auto reactor_pos = entity(w.reaction_target.building_id)->component<position_t>();
					w.current_path = find_path(pos, position_t{ reactor_pos->x, reactor_pos->y, reactor_pos->z });
					return;
				}
				else if (w.step == ai_tag_work_order::work_steps::GO_TO_WORKSHOP) {
					//std::cout << "Go to workshop\n";
					work.follow_path(w, pos, e, [&w, &e, &pos]() {
						// Cancel
						unclaim_by_id(w.current_tool);
						inventory_system::drop_item(w.current_tool, pos.x, pos.y, pos.z );
						delete_component<ai_tag_work_order>(e.id);
						//std::cout << "Bailing - no path to workshop\n";
					}, [&w, &pos, &e]() {
						// Arrived
						w.current_path.reset();
						w.step = ai_tag_work_order::work_steps::DROP_INPUT;
					});
					return;
				}
				else if (w.step == ai_tag_work_order::work_steps::DROP_INPUT) {
					//std::cout << "Drop input\n";
					if (w.current_tool == 0) std::cout << "Warning: component is unassigned at this time\n";
					inventory_system::drop_item(w.current_tool, pos.x, pos.y, pos.z );
					w.current_tool = 0;
					w.step = ai_tag_work_order::work_steps::SELECT_INPUT;
					return;
				}
				else if (w.step == ai_tag_work_order::work_steps::REACT) {
					//std::cout << "React\n";

					// Skill check, destroy inputs, create outputs
					auto stats = e.component<game_stats_t>();
					auto finder = get_reaction_def(w.reaction_target.reaction_tag);
					auto skill_check = skill_roll(e.id, *stats, rng, finder->skill, finder->difficulty);

					if (skill_check >= SUCCESS) {
						// Delete components
						std::size_t material = get_material_by_tag("plasteel");
						std::string mat_names = "";
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
						for (auto &output : finder->outputs) {
							for (int i = 0; i < output.second; ++i) {
								bool done = false;

								if (!done && finder->specials.test(special_reaction_cooking)) {
									// This is more complicated, we have to make a special item from the components.
									// The idea is to get something like Roast Asparagus
									std::cout << "Cooking Reaction - spawning " << output.first << "/" << material << "\n";
									auto new_item = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, output.first, material);
									auto item = new_item->component<item_t>();
									item->item_name = mat_names + item->item_name;
									done = true;
								}

								if (!done && finder->specials.test(special_reaction_tanning)) {
									// This is more complicated, we have to make a special item from the components.
									// The idea is to get something like Roast Asparagus
									std::cout << "Tanning Reaction - spawning " << output.first << "/" << material << "\n";
									auto new_item = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, output.first, material);
									auto item = new_item->component<item_t>();
									item->item_name = mat_names + item->item_name;
									done = true;
								}

								if (!done) {
									std::cout << "Reaction - spawning " << output.first << "/" << material << "\n";
									spawn_item_on_ground(pos.x, pos.y, pos.z, output.first, material);
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
						free_workshop(w.reaction_target.building_id);
						render::models_changed = true;
						inventory_system::inventory_has_changed();
						distance_map::refresh_blocks_map();
						particles::block_destruction_effect(pos.x, pos.y, pos.z, 1.0f, 1.0f, 1.0f, particles::PARTICLE_LUMBERJACK);

						// Become idle
						work.cancel_work_tag(e);
					}
				}
			});
		}
	}
}