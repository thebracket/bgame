#include "stdafx.h"
#include "trigger_system.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../bengine/imgui.h"
#include "../../raws/raws.hpp"
#include "../../raws/items.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../gui/log_system.hpp"
#include "../../global_assets/spatial_db.hpp"
#include "movement_system.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../render_engine/chunks/chunks.hpp"
#include "door_system.hpp"
#include "../power/power_system.hpp"
#include "../damage/damage_system.hpp"
#include "../../global_assets/rng.hpp"
#include "bengine/node_editor.hpp"
#include "raws/buildings.hpp"
#include "raws/defs/building_def_t.hpp"

using namespace bengine;

namespace systems {
	namespace triggers {

		using namespace bengine;
		using namespace region;
		using namespace logging;

		bool dirty = true;
		std::unordered_map<int, std::size_t> triggers;
		int trigger_id = 0;
		int selected_existing_link = 0;
		int new_link = 0;
		const std::string win_trigger_mgmt = std::string(ICON_FA_COG) + " Trigger Management";
		const std::string btn_remove_link = std::string(ICON_FA_MINUS) + " Remove Link";
		const std::string btn_add_link = std::string(ICON_FA_PLUS) + " Add Link";
		const std::string btn_close = std::string(ICON_FA_TIMES) + " Close";		

		thread_safe_message_queue<triggers_changed_message> triggers_changed;
		thread_safe_message_queue<request_lever_pull_message> lever_pull_requests;
		thread_safe_message_queue<leverpull_changed_message> lever_pull_changes;
		thread_safe_message_queue<trigger_details_requested> trigger_details;
		thread_safe_message_queue<lever_pulled_message> lever_pulled;

		void manage_triggers() {
			triggers_changed.process_all([](triggers_changed_message &msg) {
				//std::cout << "Received trigger notification\n";
				dirty = true;
			});

			if (dirty) {
				//std::cout << "Rebuilding trigger list\n";
				triggers.clear();
				each<entry_trigger_t, position_t>([](entity_t &e, entry_trigger_t &trigger, position_t &pos) {
					//std::cout << "Found a trigger!\n";
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

		bool show_window = true;
		int node_graph_id = -1;
		static std::vector<std::unique_ptr<node_graph::node_t>> all_nodes;

		void decorate_node(entity_t &e, const name_t &n, node_graph::node_t * node)
		{
			node->name = n.first_name + std::string(" #") + std::to_string(e.id);
			node->entity_id = e.id;
			const auto gp = e.component<node_graph_position_t>();
			if (gp)
			{
				node->pos = ImVec2{ static_cast<float>(gp->x), static_cast<float>(gp->y) };
			} else
			{
				e.assign(node_graph_position_t{ static_cast<int>(node->pos.x), static_cast<int>(node->pos.y) });
			}
			const auto pos = e.component<position_t>();
			if (pos)
			{
				node->world_x = pos->x;
				node->world_y = pos->y;
				node->world_z = pos->z;
			}
		}

		void edit_triggers() {
			trigger_details.process_all([](const trigger_details_requested &msg) {
				trigger_id = msg.lever_id;
				pause_mode = PAUSED;
				game_master_mode = TRIGGER_MANAGEMENT;
			});

			if (game_master_mode != TRIGGER_MANAGEMENT) return;
			if (node_graph_id != trigger_id)
			{
				node_graph_id = trigger_id;
				all_nodes.clear();
				// TODO: Build the node graph for what already exists
				// TODO: Build the graph of available nodes

				auto x = 100;
				auto y = 100;
				std::set<int> included_items;

				// Levers
				bengine::each<lever_t, name_t>([&included_items, &x, &y] (entity_t &e, lever_t &l, name_t &n)
				{
					const auto finder = included_items.find(e.id);
					if (finder == included_items.end())
					{
						auto lever = create_node_from_name(ImVec2(x, y), "Input");
						decorate_node(e, n, lever.get());
						all_nodes.emplace_back(std::move(lever));
						x += 100;
						included_items.insert(e.id);
					}
				});

				// Pressure Plates
				bengine::each<pressure_plate_t, name_t>([&included_items, &x, &y](entity_t &e, pressure_plate_t &l, name_t &n)
				{
					const auto finder = included_items.find(e.id);
					if (finder == included_items.end())
					{
						auto lever = create_node_from_name(ImVec2(x, y), "Input");
						decorate_node(e, n, lever.get());
						all_nodes.emplace_back(std::move(lever));
						x += 100;
						included_items.insert(e.id);
					}
				});

				// Float gauges
				bengine::each<float_gauge_t, name_t>([&included_items, &x, &y](entity_t &e, float_gauge_t &l, name_t &n)
				{
					const auto finder = included_items.find(e.id);
					if (finder == included_items.end())
					{
						auto lever = create_node_from_name(ImVec2(x, y), "Input");
						decorate_node(e, n, lever.get());
						all_nodes.emplace_back(std::move(lever));
						x += 100;
						included_items.insert(e.id);
					}
				});

				// Oscillator
				bengine::each<oscillator_t, name_t>([&included_items, &x, &y](entity_t &e, oscillator_t &l, name_t &n)
				{
					const auto finder = included_items.find(e.id);
					if (finder == included_items.end())
					{
						auto lever = create_node_from_name(ImVec2(x, y), "Input");
						decorate_node(e, n, lever.get());
						all_nodes.emplace_back(std::move(lever));
						x += 100;
						included_items.insert(e.id);
					}
				});

				// Signal Processor
				bengine::each<signal_processor_t, name_t>([&included_items, &x, &y](entity_t &e, signal_processor_t &l, name_t &n)
				{
					const auto finder = included_items.find(e.id);
					if (finder == included_items.end())
					{
						std::unique_ptr<node_graph::node_t> lever;
						switch (l.processor)
						{
						case AND :
						{
							lever = create_node_from_name(ImVec2(x, y), "AND");
						} break;
						case OR:
						{
							lever = create_node_from_name(ImVec2(x, y), "OR");
						} break;
						case NOT:
						{
							lever = create_node_from_name(ImVec2(x, y), "NOT");
						} break;
						case NAND:
						{
							lever = create_node_from_name(ImVec2(x, y), "NAND");
						} break;
						case NOR:
						{
							lever = create_node_from_name(ImVec2(x, y), "NOR");
						} break;
						case EOR:
						{
							lever = create_node_from_name(ImVec2(x, y), "EOR");
						} break;
						case ENOR:
						{
							lever = create_node_from_name(ImVec2(x, y), "ENOR");
						} break;
						}
						decorate_node(e, n, lever.get());
						all_nodes.emplace_back(std::move(lever));
						x += 100;
						included_items.insert(e.id);
					}
				});

				// Outputs
				bengine::each<receives_signal_t, name_t>([&included_items, &x, &y](entity_t &e, receives_signal_t &l, name_t &n)
				{
					const auto finder = included_items.find(e.id);
					if (finder == included_items.end())
					{
						auto lever = create_node_from_name(ImVec2(x, y), "Output");
						decorate_node(e, n, lever.get());
						all_nodes.emplace_back(std::move(lever));
						x += 100;
						included_items.insert(e.id);
					}
				});

				// Rebuild connection nodes
				for (auto &node : all_nodes)
				{
					const auto e = entity(node->entity_id);
					if (e) {
						const auto r = e->component<receives_signal_t>();
						if (r) {
							for (const auto &rs : r->receives_from)
							{
								const auto remote_node_id = std::get<0>(rs);
								const auto rx = std::get<1>(rs);
								const auto ry = std::get<2>(rs);
								const auto sx = std::get<3>(rs);
								const auto sy = std::get<4>(rs);
								const auto remote_node = find_node_by_entity_id(remote_node_id, all_nodes);

								auto conn = std::make_unique<node_graph::connection_t>();
								conn->pos = ImVec2(rx, ry);

								auto conn2 = std::make_unique<node_graph::connection_t>();
								conn2->output.emplace_back(conn.get());
								conn2->pos = ImVec2(sx, sy);
								conn->input = conn2.get();

								node->input_connections.emplace_back( std::move(conn) );
								remote_node->output_connections.emplace_back(std::move(conn2));
							}
						}
					}
				}
			}

			ShowExampleAppCustomNodeGraph(&show_window, all_nodes);
			if (!show_window) {
				game_master_mode = PLAY;
				show_window = true;
				node_graph_id = -1;

				for (const auto &node : all_nodes)
				{
					const auto ne = entity(node->entity_id);
					const auto send = ne->component<sends_signal_t>();
					const auto recv = ne->component<receives_signal_t>();

					if (send) send->targets.clear();
					if (recv) recv->receives_from.clear();
				}

				// Update system to reflect node changes
				for (const auto &node : all_nodes)
				{
					std::cout << "Reading node: " << node->name << "\n";
					const auto node_entity = entity(node->entity_id);
					const auto np = node_entity->component<node_graph_position_t>();
					np->x = static_cast<int>(node->pos.x);
					np->y = static_cast<int>(node->pos.y);

					for (const auto &conn : node->input_connections)
					{
						const auto input_node = find_node_by_con(conn.get(), all_nodes);
						const auto output_node = find_node_by_con(conn->input, all_nodes);

						const auto input_e = entity(input_node->entity_id);
						const auto output_e = entity(output_node->entity_id);
						auto recipient = input_e->component<receives_signal_t>();
						auto sender = output_e->component<sends_signal_t>();
						recipient->receives_from.emplace_back(std::make_tuple(output_node->entity_id, conn->pos.x, conn->pos.y, conn->input->pos.x, conn->input->pos.y));
						sender->targets.emplace_back(input_node->entity_id);
						//std::cout << "Input Connection to " << find_node_by_con(conn.get(), all_nodes)->name << "\n";
						//std::cout << "Connection to " << find_node_by_con(conn->input, all_nodes)->name << "\n";
					}
				}
			}
		}

		void entry_trigger_firing(const systems::movement::entity_moved_message &msg) {
			/*
			//std::cout << "Received an entity move message. There are " << triggers.size() << " triggers.\n";
			const auto tile_index = mapidx(msg.destination);
			const auto finder = triggers.find(tile_index);
			if (finder != triggers.end()) {
				//std::cout << "Found a trigger\n";
				auto trigger_entity = entity(finder->second);
				if (trigger_entity) {
					const auto trigger_def = trigger_entity->component<entry_trigger_t>();
					if (trigger_def) {
						if (trigger_def->active) {
							//std::cout << "Trigger is active\n";
							// Does the trigger apply to the entity type
							auto target_entity = entity(msg.entity_id);
							if (target_entity) {
								const auto grazer = target_entity->component<grazer_ai>();
								const auto sentient = target_entity->component<sentient_ai>();

								//if (grazer) std::cout << "Target grazes\n";
								//if (sentient) std::cout << "Target is sentient\n";
								//if (settler) std::cout << "Target a settler - probably ignored\n";

								// Cages only affect hostiles and beasts
								if (trigger_def->type == TRIGGER_CAGE && (grazer || (sentient && sentient->hostile))) {
									//std::cout << "Cage triggered\n";
									const auto name = target_entity->component<name_t>();
									if (name) {
										LOG ss;
										ss.other_name(msg.entity_id)->text(" is trapped in a cage!");
										logging::log_message msg_{ ss.chars };
										logging::log(msg_);
									}

									// TODO: Add a random chance with some dex involved
									// Spawn a cage object
									auto building = trigger_entity->component<building_t>();
									const auto material = get_material_by_tag(building->built_with[0].first);
									const auto &[x,y,z] = idxmap(tile_index);
									const auto new_cage = spawn_item_on_ground_ret(x, y, z, "cage", material, 3, 100);

									// Add a stored component
									target_entity->assign(item_stored_t{ new_cage->id });

									// Remove the position component
									delete_component<position_t>(msg.entity_id);

									// Remove the trap
									delete_entity(finder->second);
									triggers_changed.enqueue(triggers_changed_message{});
								}
								else if (trigger_def->type == TRIGGER_STONEFALL && (grazer || (sentient && sentient->hostile))) {
									//std::cout << "Stonefall triggered\n";
									// Stonefalls only affect hostiles
									const auto name = target_entity->component<name_t>();
									if (name) {
										LOG ss;
										ss.other_name(msg.entity_id)->text(" is hit by a falling rock trap!");
										logging::log_message msg_{ ss.chars };
										logging::log(msg_);
									}

									// Spawn some damage!
									damage_system::inflict_damage(damage_system::inflict_damage_message{ msg.entity_id, rng.roll_dice(3, 6), "falling rocks" });

									// Remove the trap
									delete_entity(finder->second);
									triggers_changed.enqueue(triggers_changed_message{});
								}
								else if (trigger_def->type == TRIGGER_BLADE && (grazer || (sentient && sentient->hostile))) {
									//std::cout << "Blade trap triggered\n";
									// Blades only affect hostiles, and don't auto-destruct
									const auto name = target_entity->component<name_t>();
									if (name) {
										LOG ss;
										ss.other_name(msg.entity_id)->text(" is hit by a blade trap!");
										logging::log_message msg_{ ss.chars };
										logging::log(msg_);
									}

									// TODO: Add a random chance with some dex involved
									// Spawn some damage!
									damage_system::inflict_damage(damage_system::inflict_damage_message{ msg.entity_id, rng.roll_dice(3,8), "spinning blades" });

									// Extend the blades
									const auto building = target_entity->component<building_t>();
									if (building)
									{
										building->vox_model = 132;
									}
								}
								else if (trigger_def->type == TRIGGER_PRESSURE)
								{
									lever_pulled.enqueue(lever_pulled_message{ trigger_entity->id });
								}
							}
						}
					}
				}
			}*/
		}

		void pulled_levers() {
			/*
			lever_pulled.process_all([](const lever_pulled_message &msg) {
				auto lever_entity = entity(msg.lever_id);
				if (!lever_entity) return;
				auto lever_component = lever_entity->component<lever_t>();
				if (!lever_component) return;
				auto lever_building = lever_entity->component<building_t>();

				lever_component->active = !lever_component->active;
				if (lever_building) {
					if (lever_component->active) {
						lever_building->vox_model = 124;
					}
					else {
						lever_building->vox_model = 125;
					}
				}

				for (const auto &id : lever_component->targets) {
					auto target_entity = entity(id);
					if (!target_entity) break;
					systems::power::consume_power("Lever", 10);
					auto target_door = target_entity->component<construct_door_t>();
					auto target_bridge = target_entity->component<bridge_t>();
					auto target_building = target_entity->component<building_t>();

					if (target_door) {
						target_door->locked = !target_door->locked;
						doors::doors_changed();
					}
					if (target_bridge) {
						target_bridge->retracted = !target_bridge->retracted;
						if (target_bridge->retracted) {
							// Retract the bridge
							for (int i = 0; i<REGION_TILES_COUNT; ++i) {
								if (bridge_id(i) == id) {
									make_open_space(i);
									chunks::mark_chunk_dirty_by_tileidx(i);
								}
							}
						}
						else {
							// Extend the bridge!
							for (int i = 0; i<REGION_TILES_COUNT; ++i) {
								if (bridge_id(i) == id) {
									make_floor(i);
									chunks::mark_chunk_dirty_by_tileidx(i);
								}
							}
						}
						tile_recalc_all();
					}
					if (target_building && target_building->tag == "spike_trap") {
						auto receiver = target_entity->component<receives_signal_t>();
						auto target_pos = target_entity->component<position_t>();

						if (receiver) std::cout << "Has receiver\n";
						if (target_pos) std::cout << "Has position\n";

						if (receiver && target_pos) {
							receiver->active = !receiver->active;
							if (receiver->active) {
								target_building->vox_model = 130;
								// Attack everything in the tile
								const auto &[x,y,z] = idxmap(mapidx(*target_pos));
								auto visible_here = entity_octree.find_by_loc(octree_location_t{ x, y, z, 0 });
								for (const auto &v : visible_here) {
									auto victim_entity = entity(v);
									if (victim_entity) {
										auto health = victim_entity->component<health_t>();
										if (health) {
											systems::damage_system::inflict_damage(systems::damage_system::inflict_damage_message{ v, rng.roll_dice(2,8), "Floor Spikes" });
										}
									}
								}
							}
							else {
								target_building->vox_model = 129;
							}
						}
					}
				}
			}
			);*/
		}

		void run(const double &duration_ms) {
			manage_triggers();
			pull_levers();
			pulled_levers();
		}
	}
}