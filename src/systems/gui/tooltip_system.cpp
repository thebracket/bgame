#include "stdafx.h"
#include "tooltip_system.hpp"
#include "../mouse.hpp"
#include "../../planet/region/region.hpp"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../../raws/materials.hpp"
#include "../../raws/plants.hpp"
#include "../../raws/defs/plant_t.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../components/name.hpp"
#include "../../components/position.hpp"
#include "../../components/items/item.hpp"
#include "../../components/construct_container.hpp"
#include "../../components/items/item_stored.hpp"
#include "../../components/items/item_quality.hpp"
#include "../../components/items/item_wear.hpp"
#include "../../components/building.hpp"
#include "../../components/bridge.hpp"
#include "../../components/claimed_t.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../global_assets/game_config.hpp"
#include "../../stdafx.h"

namespace systems {
	namespace tooltips {
		void run(const double &duration_ms) {
			using namespace region;
			using namespace bengine;

			if (mouse_wx != 0 && mouse_wy != 0 && mouse_wz != 0) {
				const int world_x = mouse_wx;
				const int world_y = mouse_wy;
				const int world_z = mouse_wz;
				if (world_x < 0 || world_x > REGION_WIDTH - 1 || world_y < 0 || world_y > REGION_HEIGHT - 1) return;
				const int tile_idx = mapidx(world_x, world_y, world_z);
				if (!region::revealed(tile_idx)) return;

				std::vector<std::string> lines;

				// Debug flags
				{
					std::stringstream ss;
					if (solid(tile_idx)) ss << "Solid-";
					if (opaque(tile_idx)) ss << "Opaque-";
					if (flag(tile_idx, CAN_GO_DOWN)) ss << "Down-";
					if (flag(tile_idx, CAN_GO_UP)) ss << "Up-";
					if (flag(tile_idx, CAN_GO_NORTH)) ss << "North-";
					if (flag(tile_idx, CAN_GO_EAST)) ss << "East-";
					if (flag(tile_idx, CAN_GO_WEST)) ss << "West-";
					if (flag(tile_idx, CAN_GO_SOUTH)) ss << "South-";
					if (flag(tile_idx, CAN_STAND_HERE)) ss << "Stand";
					lines.push_back(ss.str());
				}

				if (water_level(tile_idx)>0) lines.push_back(std::string("Water level: " + std::to_string(water_level(tile_idx))));

				{ // Base tile type
					std::stringstream ss;
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
					lines.push_back(ss.str());
				}
				// TODO: Fix me!
				/*if (region::tile_type(tile_idx) == tile_type::FLOOR && !flag(tile_idx, CONSTRUCTION)) {
					if (veg_type(tile_idx) > 0) {
						std::stringstream ss;
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
							const std::string harvest_to = plant->provides[veg_lifecycle(tile_idx)];
							if (harvest_to != "none") ss << " - " << harvest_to;
							ss << ")";
							lines.push_back(ss.str());
						}
					}
				}*/

				// Named entities in the location
				each<name_t, position_t>([&lines, &world_x, &world_y](entity_t &entity, name_t &name, position_t &pos) {
					if (pos.x == world_x && pos.y == world_y && pos.z == camera_position->region_z) {
						lines.push_back(name.first_name + std::string(" ") + name.last_name);
					}
				});
				// Items on the ground
				std::map<std::string, int> items;
				each<item_t, position_t>([&world_x, &world_y, &items](entity_t &entity, item_t &item, position_t &pos) {
					if (pos.x == world_x && pos.y == world_y && pos.z == camera_position->region_z) {
						auto finder = items.find(item.item_name);
						if (finder == items.end()) {
							std::string claimed = "";
							if (entity.component<claimed_t>() != nullptr) claimed = " (c)";
							std::string quality = "";
							std::string wear = "";

							auto qual = entity.component<item_quality_t>();
							auto wr = entity.component<item_wear_t>();
							if (qual) quality = std::string(" (") + qual->get_quality_text() + std::string(" quality)");
							if (wr) wear = std::string(" (") + wr->get_wear_text();

							items[item.item_name + claimed + quality + wear] = 1;
						}
						else {
							++finder->second;
						}
					}
				});
				for (auto it = items.begin(); it != items.end(); ++it) {
					std::string n = std::to_string(it->second);
					lines.push_back(n + std::string("x ") + it->first);
				}

				// Buildings
				each<building_t, position_t>([&lines, &world_x, &world_y, &items](entity_t &building_entity, building_t &building, position_t &pos) {
					bool on_building = false;

					if (pos.z == camera_position->region_z) {
						if (pos.x == world_x && pos.y == world_y) {
							on_building = true;
						}
						else {
							// Special case because 3x3 use the mid-point as center
							if (building.height == 3 && building.width == 3 && world_x >= pos.x - 1 && world_x <= pos.x + 1 && world_y >= pos.y - 1 && world_y <= pos.y + 1) {
								on_building = true;
							}
							else if (building.height != 1 && building.width != 1 && world_x >= pos.x && world_x <= pos.x + building.width && world_y >= pos.y && world_y <= pos.y + building.height) {
								on_building = true;
							}
						}
					}

					if (on_building) {
						// It's building and we can see it
						auto finder = get_building_def(building.tag);
						std::string building_name = "Unknown Building";
						if (finder != nullptr) {
							if (building.complete) {
								building_name = finder->name + std::string(" (") + std::to_string(building.hit_points)
									+ std::string("/") + std::to_string(building.max_hit_points) + std::string(")");
							}
							else {
								building_name = std::string("...") + finder->name;
							}
						}
						lines.push_back(building_name);

						auto container = building_entity.component<construct_container_t>();
						if (container) {
							//std::cout << "It's a container\n";
							items.clear();
							each<item_t, item_stored_t>([&items, &world_x, &world_y, &building_entity](entity_t &entity, item_t &item, item_stored_t &stored) {
								if (stored.stored_in == building_entity.id) {
									auto finder = items.find(item.item_name);
									if (finder == items.end()) {
										items[item.item_name] = 1;
									}
									else {
										++finder->second;
									}
								}
							});

							for (auto it = items.begin(); it != items.end(); ++it) {
								std::string n = std::to_string(it->second);
								lines.push_back(n + std::string("x ") + it->first);
							}
						}
					}
				});
				if (stockpile_id(mapidx(world_x, world_y, camera_position->region_z))>0) {
					lines.push_back(std::string("Stockpile #") + std::to_string(stockpile_id(mapidx(world_x, world_y, camera_position->region_z))));
				}
				if (bridge_id(mapidx(world_x, world_y, camera_position->region_z))>0) {
					auto be = entity(bridge_id(mapidx(world_x, world_y, camera_position->region_z)));
					if (be) {
						auto bc = be->component<bridge_t>();
						if (bc) {
							if (bc->complete) {
								lines.push_back("Bridge");
							}
							else {
								lines.push_back("... Bridge");
							}
						}
					}
				}

				float longest = 0.0F;
				for (const std::string &s : lines) {
					const auto render_size = ImGui::CalcTextSize(s.c_str());
					if (render_size.x > longest) longest = render_size.x;
				}

				// TODO - dynamic placement
				bool right_align = true;

				if (!right_align) {
					ImGui::SetNextWindowPos({ static_cast<float>(mouse_x + 35), static_cast<float>(mouse_y) });
				}
				else {
					ImGui::SetNextWindowPos({ static_cast<float>(mouse_x) - (longest + 35.0F), static_cast<float>(mouse_y) });
				}
				ImGui::Begin("Tooltip", nullptr, ImVec2{ 600, 400 }, 100.0,
					ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoTitleBar);
				for (const std::string &s : lines) {
					ImGui::Text("%s", s.c_str());
				}
				ImGui::End();
			}
		}
	}
}