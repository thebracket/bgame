#include "stdafx.h"
#include "design_architecture.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../global_assets/architecture_designations.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../mouse.hpp"
#include "../helpers/inventory_assistant.hpp"
#include "../../planet/region/region.hpp"
#include "../ai/architecture_system.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../bengine/filesystem.hpp"
#include "boost/filesystem.hpp"
#include "architecture_template.hpp"
#include "../keydamper.hpp"
#include "../../bengine/btabs.hpp"
#include <cereal/archives/xml.hpp>

namespace systems {
	namespace design_architecture {
		std::map<int, uint8_t> architecture_cursors; // int = mapidx, uint8_t cursor type.

		static int architecture_mode = 0;
		static int arch_width = 1;
		static int arch_height = 1;
		static bool arch_filled = true;
		static bool arch_available = false;
		static bool arch_possible = true;
		static int arch_x = 0;
		static int arch_y = 0;
		static const std::string win_architect = std::string(ICON_FA_BUILDING) + " Architecture";
		static bool showing_save_template = false;
		static const std::string win_arch_template = std::string(ICON_FA_FLOPPY_O) + " Save Architecture Template";
		static const std::string btn_arch_template = std::string(ICON_FA_FLOPPY_O) + " Save Architecture Template##button";
		static const std::string btn_close = std::string(ICON_FA_TIMES) + " Close without saving";


		static int calc_required_blocks(const int &w, const int &h, const bool &filled) {
			if (!filled) {
				return w*h;
			}
			else {
				return (w * 2) + (h * 2) - 4;
			}
		}

		static architecture_template_t current_template;
		static std::vector<architecture_template_t> architecture_templates;
		static bool loaded_templates = false;
		static char filename[254];
		static char title[254];
		static int selected_template = 0;
		static std::string template_list;
		static int previous_template = 0;

		static bool hasEnding(std::string const &fullString, std::string const &ending) {
			if (fullString.length() >= ending.length()) {
				return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
			}
			else {
				return false;
			}
		}

		static void load_architecture_templates() {
			using namespace boost::filesystem;

			architecture_templates.clear();
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
					if (hasEnding(current_file, ".arch-template")) {
						std::ifstream load_file(current_file);
						cereal::XMLInputArchive iarchive(load_file);
						iarchive(current_template);
						architecture_templates.push_back(current_template);
					}
				}
			}
			if (!architecture_templates.empty()) {
				current_template = architecture_templates[0];
				template_list = "";
				for (const auto &t : architecture_templates) {
					template_list += t.title;
					template_list += '\0';
				}
				template_list += '\0';
				template_list += '\0';
			}
		}

		static void build_cursors()
		{
			architecture_cursors.clear();
			for (const auto &a : architecture_designations->architecture)
			{
				const auto[x, y, z] = idxmap(a.first);
				if (z == camera_position->region_z)
				{
					architecture_cursors[a.first] = a.second;
				}
			}

			if (architecture_mode == 7)
			{
				// Template
				for (const auto &b : current_template.targets) {
					architecture_cursors[mapidx(mouse_wx + b.x, mouse_wy + b.y, mouse_wz + b.z)] = b.mode;
				}
			}
			else if (architecture_mode == 0 || architecture_mode == 1 || architecture_mode == 6)
			{
				for (auto ay = 0; ay < arch_height; ++ay) {
					for (auto ax = 0; ax < arch_width; ++ax)
					{
						if (arch_filled) {
							architecture_cursors[mapidx(mouse_wx + ax, mouse_wy + ay, mouse_wz)] = architecture_mode;
						}
						else
						{
							if (ay == 0 || ay == arch_height - 1 || ax == 0 || ax == arch_width - 1)
							{
								architecture_cursors[mapidx(mouse_wx + ax, mouse_wy + ay, mouse_wz)] = architecture_mode;
							}
						}
					}
				}
			}
			else
			{
				architecture_cursors[mapidx(mouse_wx, mouse_wy, mouse_wz)] = architecture_mode;
			}
		}

		static void handle_design()
		{
			using namespace inventory;
			using namespace bengine;
			using namespace region;

			arch_available = true; // We're always allowing this to enable future planning mode

			arch_x = mouse_wx;
			arch_y = mouse_wy;
			const int world_x = arch_x;
			const int world_y = arch_y;

			if (left_click && arch_possible) {
				if (architecture_mode != 7) {
					// Build!
					std::size_t bridge_id = 0;
					if (architecture_mode == 6) {
						const auto new_bridge = create_entity()->assign(bridge_t{});
						bridge_id = new_bridge->id;
					}

					for (int y = world_y; y < world_y + arch_height; ++y) {
						for (int x = world_x; x < world_x + arch_width; ++x) {
							if (arch_filled) {
								const int idx = mapidx(x, y, mouse_wz);
								architecture_designations->architecture[idx] = architecture_mode;
								if (architecture_mode == 6) set_bridge_id(idx, bridge_id);
								//emit(map_dirty_message{});
							}
							else {
								bool interior = false;
								if (x > world_x && x<world_x + arch_width && y>world_y && y < world_y + arch_height) interior = true;
								if (x == world_x) interior = false;
								if (x == world_x + arch_width - 1) interior = false;
								if (y == world_y) interior = false;
								if (y == world_y + arch_height - 1) interior = false;
								if (!interior) {
									const int idx = mapidx(x, y, mouse_wz);
									architecture_designations->architecture[idx] = architecture_mode;
									if (architecture_mode == 6) set_bridge_id(idx, bridge_id);
								}
							}
						}
					}
				} else
				{
					// Handle template
					for (const auto &b : current_template.targets) {
						architecture_designations->architecture[mapidx(mouse_wx + b.x, mouse_wy + b.y, mouse_wz + b.z)] = b.mode;
					}
				}
				architecture_system::architecture_map_changed();
			}
			if (right_click) {
				// Erase
				const auto idx = mapidx(world_x, world_y, mouse_wz);
				const auto finder = architecture_designations->architecture.find(idx);
				if (finder != architecture_designations->architecture.end()) {
					if (finder->second == 6) {
						// Bridge - remove all of it
						const std::size_t bid = bridge_id(idx);
						if (bid > 0) {
							delete_bridge(bid);
						}
						for (auto it = architecture_designations->architecture.begin(); it != architecture_designations->architecture.end(); ++it) {
							if (it->second == 6 && bridge_id(it->first) == bid) {
								architecture_designations->architecture.erase(it->first);
							}
						}
					}
					architecture_designations->architecture.erase(idx);
					architecture_system::architecture_map_changed();
				}
			}

			arch_possible = true;

		}

		const char * arch_modes = "(W) Wall\0(F) Floor\0(U) Up Ladder\0(J) Down Ladder\0(I) Up/Down Ladder\0(R) Ramp Up\0(B) Bridge\0(T) Template\0\0";

		void run(const double &duration_ms) {
			using namespace inventory;
			using namespace bengine;
			using namespace region;

			if (!loaded_templates) {
				load_architecture_templates();
			}

			// Populate the cursors
			build_cursors();

			// GUI
			const int available_blocks = blocks_available() - architecture_designations->architecture.size();
			const auto required_blocks = calc_required_blocks(arch_width, arch_height, arch_filled);
			const auto materials_available = (required_blocks <= available_blocks);
			const auto block_availability = std::string("Available building blocks: ") + std::to_string(available_blocks) +	std::string(" (Required: ") + std::to_string(required_blocks) + std::string(")");

			if (!showing_save_template) {
				ImGui::Begin(win_architect.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
				if (materials_available) {
					ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f }, "%s", block_availability.c_str());
				}
				else {
					ImGui::TextColored(ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f }, "%s", block_availability.c_str());
				}
				ImGui::Text("Left click to build, right click to clear tile.");


				// Options for wall/floor/up/down/updown/ramp/bridge
				ImGui::Text("Architect Mode:");
				ImGui::SameLine();
				ImGui::Combo("##ArchMode", &architecture_mode, arch_modes);

				if (is_key_down(GLFW_KEY_W)) architecture_mode = 0;
				if (is_key_down(GLFW_KEY_F)) architecture_mode = 1;
				if (is_key_down(GLFW_KEY_U)) architecture_mode = 2;
				if (is_key_down(GLFW_KEY_J)) architecture_mode = 3;
				if (is_key_down(GLFW_KEY_I)) architecture_mode = 4;
				if (is_key_down(GLFW_KEY_R)) architecture_mode = 5;
				if (is_key_down(GLFW_KEY_B)) architecture_mode = 6;
				if (is_key_down(GLFW_KEY_T)) architecture_mode = 7;

				// Size options
				if (architecture_mode == 7)
				{
					if (architecture_templates.empty()) {
						ImGui::Text("There are no templates. You should make one!");
					}
					else {
						ImGui::Text("Selected brush template:");
						ImGui::SameLine();
						ImGui::Combo("##template", &selected_template, template_list.c_str());
						if (selected_template != previous_template) {
							previous_template = selected_template;
							current_template = architecture_templates[selected_template];
						}
					}
				} 
				else if (architecture_mode == 0 || architecture_mode == 1 || architecture_mode == 6) {
					bengine::render_width_control(arch_width, 1, 20);
					ImGui::SameLine();
					bengine::render_height_control(arch_height, 1, 20);

					if (architecture_mode != 6) {
						ImGui::SameLine();
						ImGui::Checkbox("Filled? (H)", &arch_filled);
						if (is_key_down(GLFW_KEY_H)) arch_filled = !arch_filled;
					}
				}
				else {
					arch_width = 1;
					arch_height = 1;
					arch_filled = true;
				}
				if (ImGui::Button(btn_arch_template.c_str())) {
					showing_save_template = true;
				}
				ImGui::End();

				// Pass through to render system
				handle_design();
			} else
			{
				// Save mode
				ImGui::Begin(win_arch_template.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
				if (ImGui::Button(btn_close.c_str())) {
					showing_save_template = false;
				}
				const auto n_elements = static_cast<int>(architecture_designations->architecture.size());

				ImGui::Text("You have %d mining targets in this template.", n_elements);
				ImGui::Text("Please enter a filename:");
				ImGui::SameLine();
				ImGui::InputText("##fn", (char *)&filename, 254);
				ImGui::Text("Please enter a title:");
				ImGui::SameLine();
				ImGui::InputText("##title", (char *)&title, 254);

				// TODO: Validation!
				if (ImGui::Button(btn_arch_template.c_str())) {
					current_template.title = std::string(title);
					const auto home = get_save_path();
					const std::string fn(filename);
					//const std::string filename = home + std::string(filename) + std::string(".mine-template");
					const std::string filename = home + std::string("/") + fn + std::string(".arch-template");
					//std::cout << "Saving template as: " << filename << "\n";

					int min_x = 1000;
					int min_y = 1000;
					int min_z = 1000;

					for (const auto &target : architecture_designations->architecture) {
						auto[x, y, z] = idxmap(target.first);
						if (min_x > x) min_x = x;
						if (min_y > y) min_y = y;
						if (min_z > z) min_z = z;
					}

					current_template.targets.clear();
					for (const auto &target : architecture_designations->architecture) {
						auto[x, y, z] = idxmap(target.first);
						const auto mode = target.second;
						current_template.targets.push_back(architecture_target{ x - min_x, y - min_y, z - min_z, mode });
					}

					{
						std::ofstream save_file(filename);
						cereal::XMLOutputArchive oarchive(save_file);
						oarchive(current_template);
					}
					load_architecture_templates();
					showing_save_template = false;

				}
				ImGui::End();
			}

		}
	}
}
