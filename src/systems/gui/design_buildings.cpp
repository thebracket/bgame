#include "design_buildings.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../mouse.hpp"
#include "../../global_assets/game_building.hpp"
#include "../helpers/inventory_assistant.hpp"
#include "../../render_engine/vox/renderables.hpp"
#include "../../stdafx.h"

namespace systems {
	namespace design_buildings {

		using namespace buildings;

		int selected_building = 0;
		const std::string win_building = std::string(ICON_FA_BUILDING_O) + " Building";

		void run(const double &duration_ms) {
			render::models_changed = true; // Models always change while in design mode - buildings

			std::vector<std::pair<std::string, std::string>> buildings;

			bool rendered_selected = false;
			auto available_buildings = inventory::get_available_buildings();
			for (const available_building_t &building : available_buildings) {
				if (has_build_mode_building && build_mode_building.tag == building.tag) rendered_selected = true;
				buildings.emplace_back(std::make_pair(building.tag, building.get_name()));
			}
			std::vector<const char *> building_listbox_items(buildings.size());
			for (int i = 0; i<buildings.size(); ++i) {
				building_listbox_items[i] = buildings[i].second.c_str();
			}

			ImGui::Begin(win_building.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
			ImGui::ListBox("", &selected_building, &building_listbox_items.at(0), buildings.size(), 10);
			ImGui::End();

			if (!rendered_selected) {
				has_build_mode_building = false;
			}
			if (buildings.size() > 0 && selected_building < available_buildings.size()) {
				build_mode_building = available_buildings[selected_building];
				has_build_mode_building = true;
			}
		}
	}
}