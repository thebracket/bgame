#include "../../global_assets/game_designations.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../mouse.hpp"
#include "../helpers/inventory_assistant.hpp"
#include "../../components/buildings/bridge.hpp"
#include "../../planet/region/region.hpp"
#include "../ai/distance_map_system.hpp"
#include "../ai/architecture_system.hpp"

namespace systems {
	namespace design_architecture {
		static int architecture_mode = 0;
		static int arch_width = 1;
		static int arch_height = 1;
		static bool arch_filled = true;
		static bool arch_available = false;
		static bool arch_possible = true;
		static int arch_x = 0;
		static int arch_y = 0;
		static const std::string win_architect = std::string(ICON_FA_BUILDING) + " Architecture";

		static int calc_required_blocks(const int &w, const int &h, const bool &filled) {
			if (!filled) {
				return w*h;
			}
			else {
				return (w * 2) + (h * 2) - 4;
			}
		}

		void run(const double &duration_ms) {
			using namespace inventory;
			using namespace bengine;
			using namespace region;

			const int available_blocks = blocks_available() - designations->architecture.size();
			const auto required_blocks = calc_required_blocks(arch_width, arch_height, arch_filled);
			const auto materials_available = (required_blocks <= available_blocks);
			const auto block_availability = std::string("Available building blocks: ") + std::to_string(available_blocks) +	std::string(" (Required: ") + std::to_string(required_blocks) + std::string(")");

			ImGui::Begin(win_architect.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
			if (materials_available) {
				ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f }, "%s", block_availability.c_str());
			}
			else {
				ImGui::TextColored(ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f }, "%s", block_availability.c_str());
			}
			ImGui::Text("Left click to build, right click to clear tile");

			// Options for wall/floor/up/down/updown/ramp/bridge
			ImGui::RadioButton("Wall", &architecture_mode, 0); ImGui::SameLine();
			ImGui::RadioButton("Floor", &architecture_mode, 1); ImGui::SameLine();
			ImGui::RadioButton("Up", &architecture_mode, 2); ImGui::SameLine();
			ImGui::RadioButton("Down", &architecture_mode, 3);
			ImGui::RadioButton("Up/Down", &architecture_mode, 4); ImGui::SameLine();
			ImGui::RadioButton("Ramp", &architecture_mode, 5); ImGui::SameLine();
			ImGui::RadioButton("Bridge", &architecture_mode, 6);

			// Size options
			if (architecture_mode == 0 || architecture_mode == 1 || architecture_mode == 6) {
				ImGui::InputInt("Width", &arch_width);
				ImGui::InputInt("Height", &arch_height);
				if (architecture_mode != 6) {
					ImGui::Checkbox("Filled?", &arch_filled);
				}
			}
			else {
				arch_width = 1;
				arch_height = 1;
				arch_filled = true;
			}
			ImGui::End();

			// Pass through to render system
			arch_available = true; // We're always allowing this to enable future planning mode

			arch_x = mouse_wx;
			arch_y = mouse_wy;
			const int world_x = arch_x;
			const int world_y = arch_y;

			if (left_click && arch_possible) {
				// Build!
				std::size_t bridge_id = 0;
				if (architecture_mode == 6) {
					const auto new_bridge = create_entity()->assign(bridge_t{});
					bridge_id = new_bridge->id;
				}

				for (int y = world_y; y<world_y + arch_height; ++y) {
					for (int x = world_x; x < world_x + arch_width; ++x) {
						if (arch_filled) {
							const int idx = mapidx(x, y, mouse_wz);
							designations->architecture[idx] = architecture_mode;
							if (architecture_mode == 6) set_bridge_id(idx, bridge_id);
							//emit(map_dirty_message{});
							architecture_system::architecture_map_changed();
						}
						else {
							bool interior = false;
							if (x>world_x && x<world_x + arch_width && y>world_y && y<world_y + arch_height) interior = true;
							if (x == world_x) interior = false;
							if (x == world_x + arch_width - 1) interior = false;
							if (y == world_y) interior = false;
							if (y == world_y + arch_height - 1) interior = false;
							if (!interior) {
								const int idx = mapidx(x, y, mouse_wz);
								designations->architecture[idx] = architecture_mode;
								if (architecture_mode == 6) set_bridge_id(idx, bridge_id);
								architecture_system::architecture_map_changed();
							}
						}
					}
				}
			}
			if (right_click) {
				// Erase
				const auto idx = mapidx(world_x, world_y, mouse_wz);
				const auto finder = designations->architecture.find(idx);
				if (finder != designations->architecture.end()) {
					if (finder->second == 6) {
						// Bridge - remove all of it
						const std::size_t bid = bridge_id(idx);
						if (bid > 0) {
							delete_bridge(bid);
						}
						for (auto it = designations->architecture.begin(); it != designations->architecture.end(); ++it) {
							if (it->second == 6 && bridge_id(it->first) == bid) {
								designations->architecture.erase(it->first);
							}
						}
					}
					designations->architecture.erase(idx);
					architecture_system::architecture_map_changed();
				}
			}

			arch_possible = true;
		}
	}
}
