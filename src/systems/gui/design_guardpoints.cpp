#include "design_guardpoints.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../mouse.hpp"
#include "../../planet/region/region.hpp"
#include <algorithm>

using namespace tile_flags;

namespace systems {
	namespace design_guardpoints {
		const std::string win_guard = std::string(ICON_FA_USER_CIRCLE) + " Guard Posts";

		void run(const double &duration_ms) {
			using namespace region;

			ImGui::Begin(win_guard.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
			ImGui::Text("Click a tile to guard it, right click to remove guard status.");
			ImGui::End();

			const int world_x = mouse_wx;
			const int world_y = mouse_wy;
			const int world_z = mouse_wz;

			const auto idx = mapidx(world_x, world_y, world_z);
			if (flag(idx, CAN_STAND_HERE)) {
				if (left_click) {
					bool found = false;
					for (const auto &g : designations->guard_points) {
						if (mapidx(g.second) == idx) found = true;
					}
					if (!found) designations->guard_points.push_back(std::make_pair(false, position_t{ world_x, world_y, world_z }));
				}
				else if (right_click) {
					designations->guard_points.erase(std::remove_if(
						designations->guard_points.begin(),
						designations->guard_points.end(),
						[&idx](std::pair<bool, position_t> p) { return idx == mapidx(p.second); }
					),
						designations->guard_points.end());
				}
			}
		}
	}
}
