#include "stdafx.h"
#include "design_harvest.hpp"
#include "../../global_assets/farming_designations.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../../bengine/imgui_tabs.hpp"
#include "../mouse.hpp"
#include "../../planet/region/region.hpp"
#include "../../raws/plants.hpp"
#include "../../raws/defs/plant_t.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../ai/distance_map_system.hpp"

namespace systems {
	namespace design_harvest {
		const std::string win_harvest = std::string(ICON_FA_SHOPPING_BASKET) + " Harvesting";

		static void display_harvest() {
			using namespace region;

			std::string harvest_name = "";
			const int world_x = mouse_wx;
			const int world_y = mouse_wy;
			const int world_z = mouse_wz;

			const auto idx = mapidx(world_x, world_y, world_z);
			bool ok = true;
			if (veg_type(idx) == 0) ok = false;
			if (ok) {
				auto p = get_plant_def(veg_type(idx));
				if (p && !p->provides.empty()) {
					const std::string harvests_to = p->provides[veg_lifecycle(idx)];
					if (harvests_to == "none") {
						ok = false;
					}
					else {
						auto finder = get_item_def(harvests_to);
						if (finder != nullptr) {
							harvest_name = finder->name;
						}
					}
				}
				else {
					ok = false;
				}
			}

			if (ok && flag(idx, CAN_STAND_HERE)) {
				if (left_click) {
					bool found = false;
					for (const auto &g : farm_designations->harvest) {
						if (mapidx(g.second) == idx) found = true;
					}
					if (!found) {
						farm_designations->harvest.push_back(std::make_pair(false, position_t{ world_x, world_y, world_z }));
					}
				}
				else if (right_click) {
					farm_designations->harvest.erase(std::remove_if(
						farm_designations->harvest.begin(),
						farm_designations->harvest.end(),
						[&idx](std::pair<bool, position_t> p) { return idx == mapidx(p.second); }
					),
						farm_designations->harvest.end());
				}
			}

			
			ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s", "Click a tile to harvest it, right click to remove harvest status.");
			if (!harvest_name.empty()) {
				ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f }, "%s", "Current tile will provide: ");
				ImGui::SameLine();
				ImGui::Text("%s", harvest_name.c_str());
			}
			
		}

		static void display_farms() {

		}

		static std::string harvest_tab = std::string(ICON_FA_LEAF) + " Harvest";
		static std::string farms_tab = std::string(ICON_FA_TASKS) + " Farms";

		void run(const double &duration_ms) {
			ImGui::Begin(win_harvest.c_str(), nullptr, ImGuiWindowFlags_NoCollapse);
			ImGui::BeginTabBar("##farming#tabs");
			ImGui::DrawTabsBackground();

			if (ImGui::AddTab(harvest_tab.c_str())) {
				display_harvest();
			}
			if (ImGui::AddTab(farms_tab.c_str())) {
				display_farms();
			}

			ImGui::EndTabBar();
			ImGui::End();
		}
	}
}
