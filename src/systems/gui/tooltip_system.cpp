#include "stdafx.h"
#include "tooltip_system.hpp"
#include "../mouse.hpp"
#include "../../planet/region/region.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../raws/materials.hpp"
#include "../../raws/plants.hpp"
#include "../../raws/defs/plant_t.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../global_assets/farming_designations.hpp"
#include "../../global_assets/debug_flags.hpp"
#include "../ai/distance_map_system.hpp"
#include "../../raws/items.hpp"
#include "../keydamper.hpp"
#include <GLFW/glfw3.h>
#include "units_info_system.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../physics/trigger_system.hpp"
#include "../physics/door_system.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../global_assets/building_designations.hpp"
#include "../../global_assets/game_designations.hpp"
#include "building_info.hpp"
#include "log_system.hpp"
#include "systems/physics/gravity_system.hpp"

using namespace tile_flags;

namespace systems {
	namespace tooltips {
		static const ImVec4 color_yellow{ 1.0f, 1.0f, 0.0f, 1.0f };
		static const ImVec4 color_cyan{ 0.0f, 1.0f, 1.0f, 1.0f };
		static const ImVec4 color_green{ 0.0f, 1.0f, 0.0f, 1.0f };
		static const ImVec4 color_red{ 1.0f, 0.0f, 0.0f, 1.0f };
		static const ImVec4 color_magenta{ 1.0f, 1.0f, 0.0f, 1.0f };

		static bool context_menu = false;
		static float popup_x = 0.0f;
		static float popup_y = 0.0f;
		static int target_idx = 0;

		static inline std::pair<std::string, ImVec4> color_line(const std::string &s, ImVec4 color = ImVec4{1.0f, 1.0f, 1.0f, 1.0f})
		{
			return std::make_pair(s, color);
		}

		static void render_context_menu()
		{
			using namespace region;
			using namespace bengine;

			ImGui::SetNextWindowPos({ popup_x, popup_y });

			auto[x, y, z] = idxmap(target_idx);
			ImGui::Begin("Popup", nullptr, ImVec2{ 600, 400 }, 100.0, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoTitleBar + ImGuiWindowFlags_NoSavedSettings);
			
			if (region::tile_type(target_idx) == tile_type::FLOOR && !flag(target_idx, CONSTRUCTION) && veg_type(target_idx) > 0) {
				const auto plant = get_plant_def(veg_type(target_idx));
				if (plant != nullptr) {
					const std::string plant_menu = std::string(ICON_FA_LEAF) + std::string(" ") + plant->name;
					if (ImGui::BeginMenu(plant_menu.c_str()))
					{
						if (!plant->provides.empty())
						{
							const auto harvest = std::string(ICON_FA_LEAF) + std::string(" Harvest");
							if (ImGui::MenuItem(harvest.c_str()))
							{
								farm_designations->harvest.emplace_back(std::make_pair(false, position_t{ x, y, z }));
								context_menu = false;
							}
						}
						ImGui::EndMenu();
					}
				}
			}

			each<name_t, position_t>([&x, &y, &z](entity_t &entity, name_t &name, position_t &pos) {
				const auto is_building = entity.component<building_t>();
				if (!is_building && pos.x == x && pos.y == y && pos.z == z) {
					const auto entity_menu = std::string(ICON_FA_USER) + std::string(" ") + name.first_name + std::string(" ") + name.last_name;
					if (ImGui::BeginMenu(entity_menu.c_str()))
					{
						const auto settler = entity.component<settler_ai_t>();
						if (settler) {
							const static std::string btn_rogue = std::string(ICON_FA_USER) + " Control";
							const std::string btn_roguemode = btn_rogue + std::string("##") + std::to_string(entity.id);
							const std::string btn_viewmode = std::string(ICON_FA_USER_CIRCLE) + std::string(" View##") + std::to_string(entity.id);
							const std::string btn_followmode = std::string(ICON_FA_VIDEO_CAMERA) + std::string(" Follow##") + std::to_string(entity.id);

							if (ImGui::MenuItem(btn_roguemode.c_str()))
							{
								auto pos = entity.component<position_t>();
								if (pos) {
									camera_position->region_x = pos->x;
									camera_position->region_y = pos->y;
									camera_position->region_z = pos->z;
								}

								auto ai = entity.component<settler_ai_t>();
								if (ai) {
									ai->job_type_major = JOB_IDLE;
								}
								game_master_mode = ROGUE;
								units_ui::selected_settler = entity.id;
								context_menu = false;
							}
							if (ImGui::MenuItem(btn_viewmode.c_str()))
							{
								game_master_mode = SETTLER;
								units_ui::selected_settler = entity.id;
								context_menu = false;
							}
							if (ImGui::MenuItem(btn_followmode.c_str()))
							{
								camera->following = entity.id;
								game_master_mode = PLAY;
								context_menu = false;
							}
							ImGui::EndMenu();
						}
					}
				}
			});

			const auto building_on_tile = get_building_id(target_idx);
			if (building_on_tile > 0) {
				auto building_entity = entity(building_on_tile);
				if (building_entity) {
					const auto building = building_entity->component<building_t>();
					std::string building_name = "Unknown Building";
					if (building) {
						const auto finder = get_building_def(building->tag);
						if (finder) {
							if (building->complete) {
								building_name = finder->name + std::string(" (") + std::to_string(building->hit_points)
									+ std::string("/") + std::to_string(building->max_hit_points) + std::string(")");

								if (building_entity->component<claimed_t>()) building_name += " (c)";
							}
							else {
								building_name = std::string("(") + finder->name + std::string(") - Incomplete");
							}
						}
						const std::string building_menu = std::string(ICON_FA_BUILDING) + std::string(" ") + building_name;
						if (ImGui::BeginMenu(building_menu.c_str()))
						{
							if (ImGui::MenuItem("Building Information"))
							{
								game_master_mode = BUILDING_INFO;
								building_info::selected_building = building_on_tile;
								context_menu = false;
							}
							if (ImGui::MenuItem("Deconstruct"))
							{
								designations->deconstructions.emplace_back(true, building_entity->id);
								context_menu = false;
							}

							const auto blever = building_entity->component<sends_signal_t>();
							if (blever)
							{
								if (ImGui::MenuItem("Pull Lever"))
								{
									triggers::lever_pull_requests.enqueue(triggers::request_lever_pull_message{ building_entity->id });
									context_menu = false;
								}
								if (ImGui::MenuItem("Manage Circuit"))
								{
									triggers::trigger_details.enqueue(triggers::trigger_details_requested{ building_entity->id });
									game_master_mode = TRIGGER_MANAGEMENT;
									pause_mode = PAUSED;
									context_menu = false;
								}
							}

							const auto bdoor = building_entity->component<construct_door_t>();
							if (bdoor)
							{
								if (bdoor->locked)
								{
									if (ImGui::MenuItem("Open Door"))
									{
										bdoor->locked = false;
										doors::doors_changed();
										context_menu = false;
										logging::log_message lmsg{ LOG{}.text("Door opened.")->chars };
										logging::log(lmsg);
									}
								} else
								{
									if (ImGui::MenuItem("Lock Door"))
									{
										if (region::water_level(target_idx) < 2) {
											bdoor->locked = true;
											doors::doors_changed();
											context_menu = false;
											logging::log_message lmsg{ LOG{}.text("Door locked.")->chars };
											logging::log(lmsg);
										} else
										{
											logging::log_message lmsg{ LOG{}.text("Unable to lock door: too much water.")->chars };
											logging::log(lmsg);
										}
									}
								}
							}

							ImGui::EndMenu();
						}
					}
				}
			}

			ImGui::End();


			if (is_key_down(GLFW_KEY_ESCAPE) || left_click) context_menu = false;
		}

		void run(const double &duration_ms) {
			using namespace region;
			using namespace bengine;

			if (context_menu)
			{
				render_context_menu();
				return;
			}

			if (mouse_wx != 0 && mouse_wy != 0 && mouse_wz != 0) {
				const int world_x = mouse_wx;
				const int world_y = mouse_wy;
				const int world_z = mouse_wz;
				if (world_x < 0 || world_x > REGION_WIDTH - 1 || world_y < 0 || world_y > REGION_HEIGHT - 1) return;
				const int tile_idx = mapidx(world_x, world_y, world_z);
				if (!region::flag(tile_idx, REVEALED)) return;

				std::vector<std::pair<std::string, ImVec4>> lines;

				// Debug flags
				if (debug::show_flags)
				{
					fmt::MemoryWriter ss;
					ss << world_x << "/" << world_y << "/" << world_z << ":";
					if (flag(tile_idx, ABOVE_GROUND)) ss << "Outside-";
					if (flag(tile_idx, SOLID)) ss << "Solid-";
					if (flag(tile_idx, OPAQUE_TILE)) ss << "Opaque-";
					if (flag(tile_idx, CAN_GO_DOWN)) ss << "D-";
					if (flag(tile_idx, CAN_GO_UP)) ss << "U-";
					if (flag(tile_idx, CAN_GO_NORTH)) ss << "N-";
					if (flag(tile_idx, CAN_GO_EAST)) ss << "E-";
					if (flag(tile_idx, CAN_GO_WEST)) ss << "W-";
					if (flag(tile_idx, CAN_GO_SOUTH)) ss << "S-";
					if (flag(tile_idx, CAN_GO_NORTH_EAST)) ss << "NE-";
					if (flag(tile_idx, CAN_GO_NORTH_WEST)) ss << "NW-";
					if (flag(tile_idx, CAN_GO_SOUTH_EAST)) ss << "SE-";
					if (flag(tile_idx, CAN_GO_SOUTH_WEST)) ss << "SW-";
					if (flag(tile_idx, CAN_STAND_HERE)) ss << "Stand-";
					if (flag(tile_idx, CONSTRUCTION)) ss << "Cons-";
					if (flag(tile_idx, VISIBLE)) ss << "Vis-";
					if (gravity::supported[tile_idx]) ss << "Supported-";
					lines.emplace_back(color_line(std::string(ICON_FA_BUG) + std::string(" ") + ss.str(), color_cyan));
				}

				if (debug::show_dijkstra)
				{
					lines.emplace_back(color_line(std::string(ICON_FA_BUG) + std::string(" ") + std::to_string(distance_map::reachable_from_cordex.get(tile_idx))));
				}

				if (water_level(tile_idx) > 0) {
					lines.emplace_back(color_line(std::string(ICON_FA_SHIP) + std::string(" Water level: " + std::to_string(water_level(tile_idx))), color_cyan));
				}

				{ // Base tile type
					fmt::MemoryWriter ss;
					switch (region::tile_type(tile_idx)) {
					case tile_type::SEMI_MOLTEN_ROCK: ss << "Magma"; break;
					case tile_type::SOLID: ss << "Solid Rock (" << material_name(material(tile_idx)) << ")"; break;
					case tile_type::OPEN_SPACE: ss << "Open Space"; break;
					case tile_type::WALL: ss << "Wall (" << material_name(material(tile_idx)) << ")"; break;
					case tile_type::RAMP: ss << "Ramp (" << material_name(material(tile_idx)) << ")"; break;
					case tile_type::STAIRS_UP: ss << "Up Stairs (" << material_name(material(tile_idx)) << ")"; break;
					case tile_type::STAIRS_DOWN: ss << "Down Stairs (" << material_name(material(tile_idx)) << ")"; break;
					case tile_type::STAIRS_UPDOWN: ss << "Spiral Stairs (" << material_name(material(tile_idx)) << ")"; break;
					case tile_type::FLOOR: ss << "Floor (" << material_name(material(tile_idx)) << ")"; break;
					case tile_type::TREE_TRUNK: ss << "Tree Trunk"; break;
					case tile_type::TREE_LEAF: ss << "Tree Foliage"; break;
					case tile_type::WINDOW: ss << "Window"; break;
					case tile_type::CLOSED_DOOR: ss << "Closed Door (" << material_name(material(tile_idx)) << ")"; break;
					default: ss << "Unknown!";
					}
					if (region::tile_type(tile_idx) != tile_type::OPEN_SPACE) {
						ss << " (" << tile_hit_points(tile_idx) << ")";
					}
					lines.emplace_back(color_line(std::string(ICON_FA_INFO_CIRCLE) + std::string(" ") + ss.str()));
				}

				{
					// Farming
					const auto farm_finder = farm_designations->farms.find(tile_idx);
					if (farm_finder != farm_designations->farms.end()) {
						fmt::MemoryWriter ss;
						ss << ICON_FA_LEAF << " Farm: " << farm_finder->second.seed_type << " - ";
						switch (farm_finder->second.state) {
						case farm_steps::CLEAR: ss << "Plough"; break;
						case farm_steps::FIX_SOIL: ss << "Fix Soil"; break;
						case farm_steps::PLANT_SEEDS: ss << "Plant Seeds"; break;
						case farm_steps::GROWING: ss << "Growing"; break;
						default: { glog(log_target::GAME, log_severity::error, "Unknown farming state!"); }
						}
						ss << ". Weeded/Watered " << farm_finder->second.days_since_weeded << "/" << farm_finder->second.days_since_watered << " days ago.";
						lines.emplace_back(color_line(ss.str(), color_green));
					}
				}

				// Plants
				if (region::tile_type(tile_idx) == tile_type::FLOOR && !flag(tile_idx, CONSTRUCTION)) {
					if (veg_type(tile_idx) > 0) {
						fmt::MemoryWriter ss;
						ss << ICON_FA_LEAF << " ";
						auto plant = get_plant_def(veg_type(tile_idx));
						if (plant != nullptr) {
							ss << plant->name << " (";
							switch (veg_lifecycle(tile_idx)) {
							case 0: ss << "Germinating"; break;
							case 1: ss << "Sprouting"; break;
							case 2: ss << "Growing"; break;
							case 3: ss << "Flowering"; break;
							default: ss << "Unknown - error!";
							}
							if (debug::show_flags) {
								ss << " " << veg_ticker(tile_idx) << "/" << plant->lifecycle[veg_lifecycle(tile_idx)];
							}
							if (!plant->provides.empty()) {
								const std::string harvest_to = plant->provides[veg_lifecycle(tile_idx)];
								if (harvest_to != "none") {
									const auto harvest_result = get_item_def(harvest_to);
									if (harvest_result) {
										ss << " - provides: " << harvest_result->name;
									}
									else {
										ss << " - provides: " << harvest_to << "; WARNING - RAW TAG";
									}
								}
							}
							ss << ")";
							lines.emplace_back(color_line(ss.str(), color_green));
						}
						else {
							lines.emplace_back(color_line(std::string(ICON_FA_BUG) + std::string(" Unknown plant. Oops."), color_red));
						}
					}
				}

				// Named entities in the location
				each<name_t, position_t>([&lines, &world_x, &world_y, &world_z](entity_t &entity, name_t &name, position_t &pos) {
					auto is_building = entity.component<building_t>();
					if (!is_building && pos.x == world_x && pos.y == world_y && pos.z == world_z) {
						if (debug::show_flags)
						{
							lines.emplace_back(color_line(std::string(ICON_FA_USER) + std::string(" ") + name.first_name + std::string(" ") + name.last_name + std::string(" #") + std::to_string(entity.id), color_magenta));
						}
						else {
							lines.emplace_back(color_line(std::string(ICON_FA_USER) + std::string(" ") + name.first_name + std::string(" ") + name.last_name, color_magenta));
						}
					}
				});
				// Items on the ground
				std::map<std::string, int> items;
				each<item_t, position_t>([&world_x, &world_y, &world_z, &items](entity_t &entity, item_t &item, position_t &pos) {
					if (pos.x == world_x && pos.y == world_y && pos.z == world_z) {
						std::string claimed;
						if (entity.component<claimed_t>() != nullptr) claimed = " (c)";
						std::string quality;
						std::string wear;

						auto qual = entity.component<item_quality_t>();
						auto wr = entity.component<item_wear_t>();
						if (qual) quality = std::string(" (") + qual->get_quality_text() + std::string(" quality)");
						if (wr) wear = std::string(" (") + wr->get_wear_text() + std::string(")");
						const auto name = item.item_name + claimed + quality + wear;

						auto finder = items.find(name);
						if (finder == items.end()) {

							items[name] = 1;
						}
						else {
							++finder->second;
						}
					}
				});
				for (const auto &it : items) {
					const auto n = std::to_string(it.second);
					lines.push_back(color_line(n + std::string("x ") + it.first));
				}

				// Buildings
				const auto building_on_tile = get_building_id(tile_idx);
				if (building_on_tile > 0) {
					if (debug::show_flags) {
						lines.push_back(color_line(std::string("Building #") + std::to_string(building_on_tile)));
					}
					auto building_entity = entity(building_on_tile);
					if (building_entity) {
						const auto building = building_entity->component<building_t>();
						std::string building_name = "Unknown Building";
						if (building) {
							const auto finder = building_entity->component<name_t>();
							if (finder) {
								if (building->complete) {
									building_name = finder->first_name + std::string(" (") + std::to_string(building->hit_points)
										+ std::string("/") + std::to_string(building->max_hit_points) + std::string(")");

									if (building_entity->component<claimed_t>()) building_name += " (c)";
								}
								else {
									building_name = std::string("(") + finder->first_name + std::string(") - Incomplete");
								}
								if (debug::show_flags && building_entity->component<receives_signal_t>()) building_name += " (rs)";
							}
							lines.push_back(color_line(std::string(ICON_FA_BUILDING) + std::string(" ") + building_name, color_yellow));

							const auto container = building_entity->component<construct_container_t>();
							if (container) {
								//std::cout << "It's a container\n";
								items.clear();
								each<item_t, item_stored_t>([&items, &building_entity](entity_t &entity, item_t &item, item_stored_t &stored) {
									if (stored.stored_in == building_entity->id) {
										auto finder = items.find(item.item_name);
										if (finder == items.end()) {
											std::string claimed;
											if (entity.component<claimed_t>() != nullptr) claimed = " (c)";
											std::string quality;
											std::string wear;

											auto qual = entity.component<item_quality_t>();
											auto wr = entity.component<item_wear_t>();
											if (qual) quality = std::string(" (") + qual->get_quality_text() + std::string(" quality)");
											if (wr) wear = std::string(" (") + wr->get_wear_text() + std::string(")");

											items[item.item_name + claimed + quality + wear] = 1;
										}
										else {
											++finder->second;
										}
									}
								});

								for (const auto &it : items) {
									const auto n = std::to_string(it.second);
									lines.push_back(color_line(std::string("     ") + std::string(ICON_FA_BRIEFCASE) + std::string(" ") + n + std::string("x ") + it.first, color_yellow));
								}
							}
						}
					}
				}

				if (stockpile_id(mapidx(world_x, world_y, world_z))>0) {
					lines.push_back(color_line(std::string("Stockpile #") + std::to_string(stockpile_id(mapidx(world_x, world_y, world_z)))));
				}
				if (bridge_id(mapidx(world_x, world_y, world_z))>0) {
					auto be = entity(bridge_id(mapidx(world_x, world_y, world_z)));
					if (be) {
						const auto bc = be->component<bridge_t>();
						if (bc) {
							if (bc->complete) {
								lines.push_back(color_line("Bridge"));
							}
							else {
								lines.push_back(color_line("... Bridge"));
							}
						}
					}
				}

				auto longest = 0.0F;
				for (const auto &s : lines) {
					const auto render_size = ImGui::CalcTextSize(s.first.c_str());
					if (render_size.x > longest) longest = render_size.x;
				}

				// TODO - dynamic placement
				//const auto right_align = true;

				//if (!right_align) {
				//	ImGui::SetNextWindowPos({ static_cast<float>(mouse_x + 35), static_cast<float>(mouse_y) });
				//}
				//else {
					ImGui::SetNextWindowPos({ static_cast<float>(mouse_x) - (longest + 35.0F), static_cast<float>(mouse_y) });
				//}
				ImGui::Begin("Tooltip", nullptr, ImVec2{ 600, 400 }, 100.0,
					ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoTitleBar + ImGuiWindowFlags_NoSavedSettings);
				for (const auto &s : lines) {
					ImGui::TextColored(s.second, "%s", s.first.c_str());
				}
				ImGui::End();

				if (left_click && (game_master_mode == PLAY || game_master_mode == LOOK_MODE )) {
					context_menu = true;
					popup_x = mouse_x;
					popup_y = mouse_y;
					target_idx = mapidx(world_x, world_y, world_z);
				}
			}
		}
	}
}
