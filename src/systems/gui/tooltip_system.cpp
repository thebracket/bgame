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
#include "../../components/name.hpp"
#include "../../components/position.hpp"
#include "../../components/items/item.hpp"
#include "../../components/buildings/construct_container.hpp"
#include "../../components/items/item_stored.hpp"
#include "../../components/items/item_quality.hpp"
#include "../../components/items/item_wear.hpp"
#include "../../components/buildings/building.hpp"
#include "../../components/buildings/bridge.hpp"
#include "../../components/claimed_t.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../global_assets/farming_designations.hpp"
#include "../../global_assets/debug_flags.hpp"
#include "../ai/distance_map_system.hpp"
#include "../../raws/items.hpp"
#include "../../bengine/ecs.hpp"
#include "../helpers/targeted_flow_map.hpp"

using namespace tile_flags;

namespace systems {
	namespace tooltips {
		static const ImVec4 color_yellow{ 1.0f, 1.0f, 0.0f, 1.0f };
		static const ImVec4 color_cyan{ 0.0f, 1.0f, 1.0f, 1.0f };
		static const ImVec4 color_green{ 0.0f, 1.0f, 0.0f, 1.0f };
		static const ImVec4 color_red{ 1.0f, 0.0f, 0.0f, 1.0f };
		static const ImVec4 color_magenta{ 1.0f, 1.0f, 0.0f, 1.0f };

		static inline std::pair<std::string, ImVec4> color_line(const std::string &s, ImVec4 color = ImVec4{1.0f, 1.0f, 1.0f, 1.0f})
		{
			return std::make_pair(s, color);
		}

		void run(const double &duration_ms) {
			using namespace region;
			using namespace bengine;

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
					if (flag(tile_idx, ABOVE_GROUND)) ss << "Outside-";
					if (flag(tile_idx, SOLID)) ss << "Solid-";
					if (flag(tile_idx, OPAQUE)) ss << "Opaque-";
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
					if (flag(tile_idx, CONSTRUCTION)) ss << "Cons";
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
						default: { glog(log_target::GAME, log_severity::ERROR, "Unknown farming state!"); }
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
					if (pos.x == world_x && pos.y == world_y && pos.z == world_z) {
						lines.emplace_back(color_line(std::string(ICON_FA_USER) + std::string(" ") + name.first_name + std::string(" ") + name.last_name, color_magenta));
					}
				});
				// Items on the ground
				std::map<std::string, int> items;
				each<item_t, position_t>([&world_x, &world_y, &world_z, &items](entity_t &entity, item_t &item, position_t &pos) {
					if (pos.x == world_x && pos.y == world_y && pos.z == world_z) {
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
			}
		}
	}
}
