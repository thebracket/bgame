#include "design_buildings.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../global_assets/game_building.hpp"
#include "../helpers/inventory_assistant.hpp"
#include "../../render_engine/vox/renderables.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/reactions.hpp"
#include "../../raws/defs/reaction_t.hpp"
#include <fmt/format.h>
#include "../../bengine/btabs.hpp"

namespace systems {
	namespace design_buildings {

		using namespace buildings;

		int selected_building = 0;
		const std::string win_building = std::string(ICON_FA_BUILDING_O) + " Building";

		static void display_building_info(const std::string &tag)
		{
			const auto building_def = get_building_def(tag);

			if (building_def)
			{
				ImGui::SameLine();
				ImGui::BeginChild("info_panel", ImVec2(300, 300));

				ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s", building_def->name.c_str());
				ImGui::TextWrapped("%s", building_def->description.c_str());
				ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "%s", ICON_FA_INFO_CIRCLE);
				ImGui::SameLine();
				ImGui::TextWrapped(" Constructing this building uses the %s skill, with a difficulty of %d.", building_def->skill.first.c_str(), building_def->skill.second);
				{
					fmt::MemoryWriter ss;
					for (const auto &comp : building_def->components)
					{
						ss << comp.quantity << "x";
						const auto item = get_item_def(comp.tag);
						if (item)
						{
							ss << item->name;
						}
						else
						{
							ss << comp.tag;
						}
						ss << ", ";
					}
					const auto comps_str = ss.str();
					const auto comps_trimmed = comps_str.substr(0, comps_str.size() - 2);
					ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s Required Components:", ICON_FA_BUILDING_O);
					ImGui::SameLine();
					ImGui::TextWrapped("%s", comps_trimmed.c_str());
				}

				{
					auto found_one = false;
					fmt::MemoryWriter ss;
					each_reaction([&ss, &building_def, &found_one](const std::string &rtag, const reaction_t * reaction)
					{
						if (rtag == building_def->tag) {
							ss << reaction->name << ", ";
							found_one = true;
						}
					});
					const auto reactions_str = ss.str();
					const auto reactions_trimmed = reactions_str.substr(0, reactions_str.size() - 2);
					if (found_one) {
						ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s Enables: ", ICON_FA_WRENCH);
						ImGui::SameLine();
						ImGui::TextWrapped("%s", reactions_trimmed.c_str());
					}
				}

				if (building_def->emits_smoke) ImGui::Text("This building emits smoke.");

				ImGui::EndChild();
			}
		}

		static int selected_not_buildable = 0;

		static std::vector<const char *> building_listbox_items;
		static std::vector<std::pair<std::string, std::string>> buildings;

		static void render_available()
		{
			if (selected_building > building_listbox_items.size() - 1) selected_building = 0;
			ImGui::BeginChild("build_list", ImVec2(300, 300));
			ImGui::ListBox("", &selected_building, &building_listbox_items.at(0), buildings.size(), 10);
			ImGui::EndChild();

			if (!building_listbox_items.empty())
			{
				const auto tag = buildings[selected_building].first;
				display_building_info(tag);
			}

			const auto control_pressed = (glfwGetKey(bengine::main_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(bengine::main_window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
			if (control_pressed)
			{
				if (is_key_down(GLFW_KEY_UP))
				{
					--selected_building;
					if (selected_building < 0) selected_building = building_listbox_items.size() - 1;
				}
				if (is_key_down(GLFW_KEY_DOWN))
				{
					++selected_building;
					if (selected_building > building_listbox_items.size() - 1) selected_building = 0;
				}
			}
		}

		static void render_all()
		{
			std::vector<std::pair<std::string, std::string>> all_buildings;
			each_building_def([&all_buildings](const building_def_t * bd)
			{
				all_buildings.emplace_back(std::make_pair(bd->tag, bd->name));
			});
			std::vector<const char *> all_building_listbox_items(all_buildings.size());
			for (auto i = 0; i<static_cast<int>(all_buildings.size()); ++i) {
				all_building_listbox_items[i] = all_buildings[i].second.c_str();
			}

			ImGui::BeginChild("cant_build_list", ImVec2(300, 300));
			ImGui::ListBox("##AllBuildings", &selected_not_buildable, &all_building_listbox_items.at(0), all_building_listbox_items.size(), 10);
			ImGui::EndChild();

			if (!all_building_listbox_items.empty())
			{
				const auto tag = all_buildings[selected_not_buildable].first;
				display_building_info(tag);
			}
		}

		static bengine::btabs_t building_tabs{
			{
				bengine::btab_t{ "Available Buildings", render_available, {{ "B", GLFW_KEY_B }} },
				bengine::btab_t{ "All Buildings", render_all,{ { "A", GLFW_KEY_A } } },
			}
		};

		void run(const double &duration_ms) {
			render::models_changed = true; // Models always change while in design mode - buildings

			buildings.clear();
			auto rendered_selected = false;
			auto available_buildings = inventory::get_available_buildings();
			for (const auto &building : available_buildings) {
				if (has_build_mode_building && build_mode_building.tag == building.tag) rendered_selected = true;
				buildings.emplace_back(std::make_pair(building.tag, building.get_name()));
			}
			building_listbox_items.clear();
			building_listbox_items.resize(buildings.size());
			for (auto i = 0; i<static_cast<int>(buildings.size()); ++i) {
				building_listbox_items[i] = buildings[i].second.c_str();
			}

			ImGui::Begin(win_building.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
			bengine::render_btab_bar(building_tabs);
			ImGui::End();

			if (!rendered_selected) {
				has_build_mode_building = false;
			}
			if (!buildings.empty() && selected_building < available_buildings.size()) {
				build_mode_building = available_buildings[selected_building];
				has_build_mode_building = true;
			}
		}
	}
}
