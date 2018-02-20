#include "../../global_assets/game_designations.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../mouse.hpp"
#include "../keydamper.hpp"
#include "../ai/mining_system.hpp"
#include "../../global_assets/game_mining.hpp"
#include "../../bengine/geometry.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "mining_template.hpp"
#include "../../bengine/filesystem.hpp"
#include "boost/filesystem.hpp"
#include "../../bengine/gl_include.hpp"
#include "../helpers/targeted_flow_map.hpp"

namespace systems {
	namespace design_mining {
		std::vector<std::pair<int, uint8_t>> mining_cursor_list;

		static const std::string win_mining = std::string(ICON_FA_DIAMOND) + " Mining";
		static const std::string win_mining_template = std::string(ICON_FA_FLOPPY_O) + " Save Mining Template";
		static const std::string btn_mining_template = std::string(ICON_FA_FLOPPY_O) + " Save Mining Template##button";
		static const std::string btn_close = std::string(ICON_FA_TIMES) + " Close without saving";

		static const char * brush_options = "Box\0Circle\0Template\0\0";
		static const char * dig_modes = "Dig (d)\0Channel (c)\0Ramp (r)\0Up Ladder (u)\0Down Ladder (j)\0Up/Down Ladder (i)\0Clear (x)\0\0";

		static bool loaded_templates = false;
		static mining_template_t current_template;
		static std::vector<mining_template_t> mining_templates;
		static char filename[254];
		static char title[254];
		static int selected_template = 0;
		static std::string template_list;
		static int previous_template = 0;

		static void add_cursor_candidate(std::vector<std::pair<int, uint8_t>> &result, const int &x, const int &y, const int &z) {
			if (x > 1 && x < REGION_WIDTH - 1 && y > 1 && y < REGION_HEIGHT - 1 && z > 1 && z < REGION_DEPTH - 1) {
				// TODO: Determine viability of selection
				result.push_back(std::make_pair(mapidx(x, y, z), mining_designations->mine_mode));
			}
		}

		static void add_cursor_candidate(std::vector<std::pair<int, uint8_t>> &result, const int &x, const int &y, const int &z, const uint8_t &mode) {
			if (x > 1 && x < REGION_WIDTH - 1 && y > 1 && y < REGION_HEIGHT - 1 && z > 1 && z < REGION_DEPTH - 1) {
				// TODO: Determine viability of selection
				result.push_back(std::make_pair(mapidx(x, y, z), mode));
			}
		}

		static void get_cursor_list(std::vector<std::pair<int, uint8_t>> &result) {
			result.clear();

			if (mining_designations->brush_type == 2) {
				for (const auto &b : current_template.targets) {
					add_cursor_candidate(result, mouse_wx + b.x, mouse_wy + b.y, mouse_wz + b.z, b.mode);
				}
			}
			else {
				if (mining_designations->mine_mode == MINE_DELETE) {
					if (mining_designations->brush_type == 0) {
						// Box mode delete
						for (int y = mouse_wy; y < mouse_wy + mining_designations->brush_size_y; ++y) {
							for (int x = mouse_wx; x < mouse_wx + mining_designations->brush_size_x; ++x) {
								const int idx = mapidx(x, y, mouse_wz);
								mining_cursor_list.erase(
									std::remove_if(mining_cursor_list.begin(), mining_cursor_list.end(), [&idx](auto &i) { return i.first == idx; }),
									mining_cursor_list.end()
								);
							}
						}
					}
					else if (mining_designations->brush_type == 1) {
						// Circle mode delete
						for (int y = mouse_wy - mining_designations->radius; y < mouse_wy + mining_designations->radius; ++y) {
							for (int x = mouse_wx - mining_designations->radius; x < mouse_wx + mining_designations->radius; ++x) {
								const float distance = std::abs(bengine::distance2d(mouse_wx, mouse_wy, x, y)) + 0.5f;
								if (distance < (static_cast<float>(mining_designations->radius))) {
									const int idx = mapidx(x, y, mouse_wz);
									mining_cursor_list.erase(
										std::remove_if(mining_cursor_list.begin(), mining_cursor_list.end(), [&idx](auto &i) { return i.first == idx; }),
										mining_cursor_list.end()
									);
								}
							}
						}
					}
				} else 
				if (mining_designations->mine_mode != MINE_STAIRS_DOWN && mining_designations->mine_mode != MINE_STAIRS_UP && mining_designations->mine_mode != MINE_STAIRS_UPDOWN) {
					if (mining_designations->brush_type == 0) {
						// Box mode
						for (int y = mouse_wy; y < mouse_wy + mining_designations->brush_size_y; ++y) {
							for (int x = mouse_wx; x < mouse_wx + mining_designations->brush_size_x; ++x) {
								add_cursor_candidate(result, x, y, mouse_wz);
							}
						}
					}
					else if (mining_designations->brush_type == 1) {
						// Circle mode
						for (int y = mouse_wy - mining_designations->radius; y < mouse_wy + mining_designations->radius; ++y) {
							for (int x = mouse_wx - mining_designations->radius; x < mouse_wx + mining_designations->radius; ++x) {
								const float distance = std::abs(bengine::distance2d(mouse_wx, mouse_wy, x, y)) + 0.5f;
								if (distance < (static_cast<float>(mining_designations->radius))) {
									add_cursor_candidate(result, x, y, mouse_wz);
								}
							}
						}
					}
				}
				else {
					// Stairs mode
					if (mining_designations->mine_mode == MINE_STAIRS_UPDOWN) {
						// Up/Down is just a single tile no matter what
						add_cursor_candidate(result, mouse_wx, mouse_wy, mouse_wz);
					}
					else {
						if (mining_designations->mine_mode == MINE_STAIRS_UP) {
							int x = mouse_wx;
							int y = mouse_wy;
							int z = mouse_wz;
							add_cursor_candidate(result, x, y, z, MINE_STAIRS_UP);
							add_cursor_candidate(result, x, y, z + mining_designations->stairs_depth, MINE_STAIRS_DOWN);
							if (mining_designations->stairs_depth > 1) {
								for (int i = 1; i < mining_designations->stairs_depth; ++i) {
									add_cursor_candidate(result, x, y, z + i, MINE_STAIRS_UPDOWN);
								}
							}
						}
						else {
							int x = mouse_wx;
							int y = mouse_wy;
							int z = mouse_wz;
							add_cursor_candidate(result, x, y, z, MINE_STAIRS_DOWN);
							add_cursor_candidate(result, x, y, z - mining_designations->stairs_depth, MINE_STAIRS_UP);
							if (mining_designations->stairs_depth > 1) {
								for (int i = 1; i < mining_designations->stairs_depth; ++i) {
									add_cursor_candidate(result, x, y, z - i, MINE_STAIRS_UPDOWN);
								}
							}
						}
					}
				}
			}
		}

		inline static void add_to_yield_map(std::map<std::string, int> &yield_map, const std::string &mine_tag, const std::string &material) {
			const auto target = get_item_def(mine_tag);
			if (!target) return;
			const std::string name = material + std::string(" ") + target->name;
			if (target) {
				auto finder = yield_map.find(name);
				if (finder != yield_map.end()) {
					++finder->second;
				}
				else {
					yield_map[name] = 1;
				}
			}
		}

		static bool hasEnding(std::string const &fullString, std::string const &ending) {
			if (fullString.length() >= ending.length()) {
				return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
			}
			else {
				return false;
			}
		}

		static void load_mining_templates() {
			using namespace boost::filesystem;

			mining_templates.clear();
			loaded_templates = true;
			const std::string home_directory = get_save_path();

			path p(home_directory);
			directory_iterator end_itr;
			for (directory_iterator itr(p); itr != end_itr; ++itr)
			{
				// If it's not a directory, list it. If you want to list directories too, just remove this check.
				if (is_regular_file(itr->path())) {
					// assign current file name to current_file and echo it out to the console.
					std::string current_file = itr->path().string();
					if (hasEnding(current_file, ".mine-template")) {
						std::ifstream load_file(current_file);
						cereal::XMLInputArchive iarchive(load_file);
						iarchive(current_template);
						mining_templates.push_back(current_template);
					}
				}
			}
			if (!mining_templates.empty()) {
				current_template = mining_templates[0];
				template_list = "";
				for (const auto &t : mining_templates) {
					template_list += t.title + '\0';
				}
				template_list += '\0';
				template_list += '\0';
			}
		}

		static bool showing_save_template = false;

		void run(const double &duration_ms) {
			if (!loaded_templates) {
				load_mining_templates();
			}

			get_cursor_list(mining_cursor_list);

			// Determine yields
			std::map<std::string, int> yield_map;
			for (const auto idx : mining_cursor_list) {
				if (region::revealed(idx.first)) {
					const auto &mat_idx = region::material(idx.first);
					auto mat = get_material(mat_idx);
					if (mat) {
						if (mat->mines_to_tag.size() > 0) {
							add_to_yield_map(yield_map, mat->mines_to_tag, mat->name);
						}
						if (mat->mines_to_tag_second.size() > 0) {
							add_to_yield_map(yield_map, mat->mines_to_tag_second, mat->name);
						}
					}
				}
				else {
					auto finder = yield_map.find("Unknown");
					if (finder != yield_map.end()) {
						++finder->second;
					}
					else {
						yield_map["Unknown"] = 1;
					}
				}
			}
			fmt::MemoryWriter yields;
			for (const auto &y : yield_map) {
				yields << y.first << " x " << y.second << ". ";
			}
			const std::string yield_str = yields.str();

			if (!showing_save_template) {
				ImGui::Begin(win_mining.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);

				if (mining_designations->brush_type != 2) {
					ImGui::Text("Mining Mode:");
					ImGui::SameLine();
					ImGui::Combo("##minemode", &mining_designations->mine_mode, dig_modes);
				}

				if (mining_designations->mine_mode != MINE_STAIRS_DOWN && mining_designations->mine_mode != MINE_STAIRS_UP && mining_designations->mine_mode != MINE_STAIRS_UPDOWN) {
					// We're in digging mode
					ImGui::Text("Brush Type:");
					ImGui::SameLine();
					ImGui::Combo("##brushtype", &mining_designations->brush_type, brush_options);

					if (mining_designations->brush_type == 0) {
						ImGui::Text("Width:");
						ImGui::SameLine();
						ImGui::InputInt("##width", &mining_designations->brush_size_x, 1, 5);
						ImGui::Text("Height:");
						ImGui::SameLine();
						ImGui::InputInt("##height", &mining_designations->brush_size_y, 1, 5);
					}
					else if (mining_designations->brush_type == 1) {
						ImGui::Text("Radius:");
						ImGui::SameLine();
						ImGui::InputInt("##radius", &mining_designations->radius, 1, 5);
					}
					else if (mining_designations->brush_type == 2) {
						if (mining_templates.empty()) {
							ImGui::Text("There are no templates. You should make one!");
						}
						else {
							ImGui::Text("Selected brush template:");
							ImGui::SameLine();
							ImGui::Combo("##template", &selected_template, template_list.c_str());
							if (selected_template != previous_template) {
								previous_template = selected_template;
								current_template = mining_templates[selected_template];
							}
						}
					}
				}
				else {
					// We're in stairs mode
					ImGui::Text("Stairs Depth:");
					ImGui::SameLine();
					ImGui::InputInt("##depth", &mining_designations->stairs_depth, 1, 5);
				}

				ImGui::Text("Predicted yield: %s", yield_str.c_str());

				if (ImGui::Button(btn_mining_template.c_str())) {
					showing_save_template = true;
				}
				ImGui::End();

				// Keyboard handler			
				if (is_key_down(GLFW_KEY_D)) mining_designations->mine_mode = MINE_DIG;
				if (is_key_down(GLFW_KEY_C)) mining_designations->mine_mode = MINE_CHANNEL;
				if (is_key_down(GLFW_KEY_R)) mining_designations->mine_mode = MINE_RAMP;
				if (is_key_down(GLFW_KEY_U)) mining_designations->mine_mode = MINE_STAIRS_UP;
				if (is_key_down(GLFW_KEY_J)) mining_designations->mine_mode = MINE_STAIRS_DOWN;
				if (is_key_down(GLFW_KEY_I)) mining_designations->mine_mode = MINE_STAIRS_UPDOWN;
				if (is_key_down(GLFW_KEY_X)) mining_designations->mine_mode = MINE_DELETE;

				if (left_click) {
					for (const auto &idx : mining_cursor_list) {
						mining_designations->mining_targets[idx.first] = idx.second;
					}
					mining_system::mining_map_changed();
				}
			}
			else {
				ImGui::Begin(win_mining_template.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
				if (ImGui::Button(btn_close.c_str())) {
					showing_save_template = false;
				}
				int n_elements = static_cast<int>(mining_designations->mining_targets.size());

				ImGui::Text("You have %d mining targets in this template.", n_elements);
				ImGui::Text("Please enter a filename:");
				ImGui::SameLine();
				ImGui::InputText("##fn", (char *)&filename, 254);
				ImGui::Text("Please enter a title:");
				ImGui::SameLine();
				ImGui::InputText("##title", (char *)&title, 254);

				// TODO: Validation!
				if (ImGui::Button(btn_mining_template.c_str())) {
					current_template.title = std::string(title);
					const auto home = get_save_path();
					const std::string fn(filename);
					//const std::string filename = home + std::string(filename) + std::string(".mine-template");
					const std::string filename = home + std::string("/") + fn + std::string(".mine-template");
					//std::cout << "Saving template as: " << filename << "\n";

					int min_x = 1000;
					int min_y = 1000;
					int min_z = 1000;

					for (const auto &target : mining_designations->mining_targets) {
						auto[x, y, z] = idxmap(target.first);
						if (min_x > x) min_x = x;
						if (min_y > y) min_y = y;
						if (min_z > z) min_z = z;
					}

					current_template.targets.clear();
					for (const auto &target : mining_designations->mining_targets) {
						auto[x, y, z] = idxmap(target.first);
						auto mode = target.second;
						current_template.targets.push_back(mining_target_t{ x - min_x, y - min_y, z - min_z, mode });
					}

					{
						std::ofstream save_file(filename);
						cereal::XMLOutputArchive oarchive(save_file);
						oarchive(current_template);
					}
					load_mining_templates();
					showing_save_template = false;

				}
				ImGui::End();
			}
		}
	}
}