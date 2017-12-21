#include "stdafx.h"
#include "door_system.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../components/entry_trigger.hpp"
#include "../../components/position.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../components/lever.hpp"
#include "../../components/bridge.hpp"
#include "../../components/construct_provides_door.hpp"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../../components/receives_signal.hpp"
#include "../../components/building.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/name.hpp"
#include "../../components/item_stored.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../../components/renderable.hpp"
#include "../../components/logger.hpp"
#include "../gui/log_system.hpp"
#include "../../global_assets/spatial_db.hpp"
#include "../../components/health.hpp"
#include "movement_system.hpp"
#include "../../bengine/IconsFontAwesome.h"

using namespace bengine;

namespace systems {
	namespace triggers {

		using namespace bengine;
		using namespace region;
		using namespace logging;

		bool dirty = true;
		std::unordered_map<int, std::size_t> triggers;
		std::size_t trigger_id = 0;
		int selected_existing_link = 0;
		int new_link = 0;
		const std::string win_trigger_mgmt = std::string(ICON_FA_COG) + " Trigger Management";
		const std::string btn_remove_link = std::string(ICON_FA_MINUS) + " Remove Link";
		const std::string btn_add_link = std::string(ICON_FA_PLUS) + " Add Link";
		const std::string btn_close = std::string(ICON_FA_TIMES) + " Close";

		struct triggers_changed_message {
		};

		struct request_lever_pull_message {
			request_lever_pull_message() {}
			request_lever_pull_message(const std::size_t &lid) : lever_id(lid) {}
			std::size_t lever_id;
		};

		struct leverpull_changed_message {
		};

		struct trigger_details_requested {
			trigger_details_requested() {}
			trigger_details_requested(const std::size_t &lid) : lever_id(lid) {}
			std::size_t lever_id;
		};

		struct lever_pulled_message {
			lever_pulled_message() {}
			lever_pulled_message(const std::size_t &lid) : lever_id(lid) {}
			std::size_t lever_id;
		};

		thread_safe_message_queue<triggers_changed_message> triggers_changed;
		thread_safe_message_queue<request_lever_pull_message> lever_pull_requests;
		thread_safe_message_queue<leverpull_changed_message> lever_pull_changes;
		thread_safe_message_queue<trigger_details_requested> trigger_details;
		thread_safe_message_queue<lever_pulled_message> lever_pulled;

		void manage_triggers() {
			triggers_changed.process_all([](triggers_changed_message &msg) {
				std::cout << "Received trigger notification\n";
				dirty = true;
			});

			if (dirty) {
				std::cout << "Rebuilding trigger list\n";
				triggers.clear();
				each<entry_trigger_t, position_t>([](entity_t &e, entry_trigger_t &trigger, position_t &pos) {
					std::cout << "Found a trigger!\n";
					triggers.insert(std::make_pair(mapidx(pos), e.id));
				});
				dirty = false;
			}
		}

		void pull_levers() {
			lever_pull_requests.process_all([](request_lever_pull_message &msg) {
				designations->levers_to_pull.emplace_back(msg.lever_id);
				lever_pull_changes.enqueue(leverpull_changed_message{});
			});
		}

		void edit_triggers() {
			trigger_details.process_all([](const trigger_details_requested &msg) {
				trigger_id = msg.lever_id;
				pause_mode = PAUSED;
				game_master_mode = TRIGGER_MANAGEMENT;
			});

			if (game_master_mode != TRIGGER_MANAGEMENT) return;
			auto trigger_entity = entity(trigger_id);
			if (!trigger_entity) return;
			auto lever = trigger_entity->component<lever_t>();

			bool is_lever = false;
			if (lever) is_lever = true;
			// TODO: is_pressure_plate determination

			ImGui::Begin(win_trigger_mgmt.c_str());

			// Input options
			if (is_lever) {
				ImGui::Text("Levers have no input options; their input is triggered when a settler pulls the lever.");
			}

			// Connection options - current destinations
			std::unordered_set<std::size_t> already_linked;
			std::vector<std::pair<std::size_t, std::string>> linked_to;
			for (const auto &target : lever->targets) {
				auto target_entity = entity(target);
				if (target_entity) {
					std::string prefix = "";
					if (target_entity->component<bridge_t>()) prefix = "Bridge";
					if (target_entity->component<construct_door_t>()) prefix = "Door";
					// Spikes, etc.

					const std::string target_info = prefix + std::string(" #") + std::to_string(target);
					linked_to.emplace_back(std::pair<std::size_t, std::string>{target, target_info});
					already_linked.insert(target);
				}
			}
			std::vector<const char *> linked_to_items(linked_to.size());
			for (int i = 0; i<linked_to.size(); ++i) {
				linked_to_items[i] = linked_to[i].second.c_str();
			}
			ImGui::PushItemWidth(-1);
			ImGui::ListBox("## Existing Links", &selected_existing_link, &linked_to_items[0], linked_to.size(), 10);
			if (ImGui::Button(btn_remove_link.c_str())) {
				if (is_lever) {
					const std::size_t target_to_remove = linked_to[selected_existing_link].first;
					lever->targets.erase(std::remove_if(lever->targets.begin(), lever->targets.end(),
						[&target_to_remove](auto &n) { return target_to_remove == n; }),
						lever->targets.end());
				}
			}

			// Possible connections
			std::vector<std::pair<std::size_t, std::string>> can_link_to;
			each<receives_signal_t>([&can_link_to, &already_linked](entity_t &e, receives_signal_t &s) {
				std::string stem = "";
				bool mine = true;
				auto building_component = e.component<building_t>();
				if (building_component && building_component->civ_owner != 0) mine = false;
				if (!mine) return;
				if (already_linked.find(e.id) != already_linked.end()) return;
				if (e.component<bridge_t>()) stem = "Bridge";
				if (e.component<construct_door_t>()) stem = "Door";
				if (building_component && building_component->tag == "spike_trap") stem = "Spikes";

				const std::string target_info = stem + std::string(" #") + std::to_string(e.id);
				can_link_to.emplace_back(std::make_pair(e.id, target_info));
			});
			std::vector<const char*> can_link(can_link_to.size());
			for (int i = 0; i<can_link_to.size(); ++i) {
				can_link[i] = can_link_to[i].second.c_str();
			}
			ImGui::PushItemWidth(-1);
			ImGui::ListBox("## New Links", &new_link, &can_link[0], can_link_to.size(), 10);
			// TODO: Find a way to move the camera to show what you are picking when selecting
			if (ImGui::Button(btn_add_link.c_str())) {
				if (is_lever) lever->targets.emplace_back(can_link_to[new_link].first);
			}
			if (ImGui::Button(btn_close.c_str())) {
				game_master_mode = PLAY;
			}
			ImGui::End();
		}

		void entry_trigger_firing(const systems::movement::entity_moved_message &msg) {
			//std::cout << "Received an entity move message. There are " << triggers.size() << " triggers.\n";
			const int tile_index = mapidx(msg.destination);
			auto finder = triggers.find(tile_index);
			if (finder != triggers.end()) {
				//std::cout << "Found a trigger\n";
				auto trigger_entity = entity(finder->second);
				if (trigger_entity) {
					auto trigger_def = trigger_entity->component<entry_trigger_t>();
					if (trigger_def) {
						if (trigger_def->active) {
							//std::cout << "Trigger is active\n";
							// Does the trigger apply to the entity type
							auto target_entity = entity(msg.entity_id);
							if (target_entity) {
								auto grazer = target_entity->component<grazer_ai>();
								auto sentient = target_entity->component<sentient_ai>();

								//if (grazer) std::cout << "Target grazes\n";
								//if (sentient) std::cout << "Target is sentient\n";
								//if (settler) std::cout << "Target a settler - probably ignored\n";

								// Cages only affect hostiles and beasts
								if (trigger_def->type == trigger_cage && (grazer || (sentient && sentient->hostile))) {
									//std::cout << "Cage triggered\n";
									auto name = target_entity->component<name_t>();
									if (name) {
										LOG ss;
										ss.other_name(msg.entity_id)->text(" is trapped in a cage!");
										logging::log(logging::log_message{ ss.chars });
									}

									// TODO: Add a random chance with some dex involved
									// Spawn a cage object
									auto building = trigger_entity->component<building_t>();
									std::size_t material = get_material_by_tag(building->built_with[0].first);
									auto &[x,y,z] = idxmap(tile_index);
									auto new_cage = spawn_item_on_ground_ret(x, y, z, "cage", material);

									// Add a stored component
									target_entity->assign<item_stored_t>(new_cage->id);

									// Remove the position component
									delete_component<position_t>(msg.entity_id);

									// Remove the trap
									delete_entity(finder->second);
									triggers_changed.enqueue(triggers_changed_message{});
								}
								else if (trigger_def->type == trigger_stonefall && (grazer || (sentient && sentient->hostile))) {
									//std::cout << "Stonefall triggered\n";
									// Stonefalls only affect hostiles
									auto name = target_entity->component<name_t>();
									if (name) {
										LOG ss;
										ss.other_name(msg.entity_id)->text(" is hit by a falling rock trap!");
										logging::log(logging::log_message{ ss.chars });
									}

									// TODO: Add a random chance with some dex involved
									// Spawn some damage!
									// TODO: emit(inflict_damage_message{ msg.entity_id, rng.roll_dice(3,6), "falling rocks" });

									// Remove the trap
									delete_entity(finder->second);
									triggers_changed.enqueue(triggers_changed_message{});
								}
								else if (trigger_def->type == trigger_blade && (grazer || (sentient && sentient->hostile))) {
									//std::cout << "Blade trap triggered\n";
									// Blades only affect hostiles, and don't auto-destruct
									auto name = target_entity->component<name_t>();
									if (name) {
										LOG ss;
										ss.other_name(msg.entity_id)->text(" is hit by a blade trap!");
										logging::log(logging::log_message{ ss.chars });
									}

									// TODO: Add a random chance with some dex involved
									// Spawn some damage!
									// TODO: emit(inflict_damage_message{ msg.entity_id, rng.roll_dice(3,8), "spinning blades" });
								}
							}
						}
					}
				}
			}
		}

		void pulled_levers() {
			lever_pulled.process_all([](const lever_pulled_message &msg) {
				std::cout << "Lever pulled: " << msg.lever_id << "\n";
				auto lever_entity = entity(msg.lever_id);
				if (!lever_entity) return;
				auto lever_component = lever_entity->component<lever_t>();
				if (!lever_component) return;
				auto renderable = lever_entity->component<renderable_t>();
				std::cout << "All good on lever\n";

				lever_component->active = !lever_component->active;
				if (renderable) {
					if (lever_component->active) {
						renderable->glyph = 326;
					}
					else {
						renderable->glyph = 327;
					}
				}

				for (const auto &id : lever_component->targets) {
					auto target_entity = entity(id);
					if (!target_entity) break;
					// TODO: emit(power_consumed_message{ 10 });
					auto target_door = target_entity->component<construct_door_t>();
					auto target_bridge = target_entity->component<bridge_t>();
					auto target_building = target_entity->component<building_t>();

					if (target_door) {
						target_door->locked = !target_door->locked;
						// TODO: emit(map_changed_message{});
						// TODO: emit(door_changed_message{});
					}
					if (target_bridge) {
						target_bridge->retracted = !target_bridge->retracted;
						if (target_bridge->retracted) {
							// Retract the bridge
							for (int i = 0; i<REGION_TILES_COUNT; ++i) {
								if (bridge_id(i) == id) {
									make_open_space(i);
								}
							}
						}
						else {
							// Extend the bridge!
							for (int i = 0; i<REGION_TILES_COUNT; ++i) {
								if (bridge_id(i) == id) {
									make_floor(i);
								}
							}
						}
						tile_recalc_all();
						// TODO: emit_deferred(map_changed_message{});
					}
					if (target_building && target_building->tag == "spike_trap") {
						auto receiver = target_entity->component<receives_signal_t>();
						auto renderable = target_entity->component<renderable_t>();
						auto target_pos = target_entity->component<position_t>();
						if (receiver && renderable && target_pos) {
							receiver->active = !receiver->active;
							if (receiver->active) {
								renderable->glyph = 325;
								// Attack everything in the tile
								auto &[x,y,z] = idxmap(mapidx(*target_pos));
								std::vector<std::size_t> visible_here = entity_octree.find_by_loc(octree_location_t{ x, y, z, 0 });
								for (const auto &v : visible_here) {
									auto victim_entity = entity(v);
									if (victim_entity) {
										auto health = victim_entity->component<health_t>();
										if (health) {
											// TODO: emit_deferred(inflict_damage_message{ v, rng.roll_dice(2,8), "floor spikes" });
										}
									}
								}
							}
							else {
								renderable->glyph = 324;
							}
						}
					}
				}
			}
			);
		}

		void run(const double &duration_ms) {
			manage_triggers();
			pull_levers();
			edit_triggers();
			pulled_levers();
		}
	}
}