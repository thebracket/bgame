#include "design_lumberjack.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../mouse.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../keydamper.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../planet/region/region.hpp"

namespace systems {
	namespace design_lumberjack {
		const std::string win_chopping = std::string(ICON_FA_TREE) + " Lumberjacking";
		const std::string btn_close = std::string(ICON_FA_TIMES) + " Close";

		void run(const double &duration_ms) 
		{
			pause_mode = PAUSED;

			ImGui::Begin(win_chopping.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
			ImGui::Text("Click a tree to cut it down, right-click to clear designation.");
			if (ImGui::Button(btn_close.c_str())) {
				game_master_mode = PLAY;
			}
			ImGui::End();

			// If escape or close is clicked, leave designations mode
			if (is_key_down(GLFW_KEY_ESCAPE, false)) game_master_mode = PLAY;

			const auto idx = mapidx(mouse_wx, mouse_wy, mouse_wz);
			const auto tree_id = region::tree_id(idx);

			if (left_click && tree_id > 0) {
				// Naieve search for the base of the tree; this could be optimized a LOT
				int lowest_z = camera_position->region_z;
				const int stop_z = lowest_z - 10;

				position_t tree_pos{ mouse_wx, mouse_wy, lowest_z };
				while (lowest_z > stop_z) {
					for (int y = -10; y<10; ++y) {
						for (int x = -10; x<10; ++x) {
							const int tree_idx = mapidx(mouse_wx + x, mouse_wy + y, lowest_z);
							if (region::tree_id(tree_idx) == tree_id) {
								tree_pos.x = mouse_wx + x;
								tree_pos.y = mouse_wy + y;
								tree_pos.z = lowest_z;
								// TODO: Emit Particles
							}
						}
					}
					--lowest_z;
				}

				designations->chopping[(int)tree_id] = tree_pos;
			}
			else if (right_click && tree_id > 0) {
				designations->chopping.erase((int)tree_id);
			}
		}
	}
}