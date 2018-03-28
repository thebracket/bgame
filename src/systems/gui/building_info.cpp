#include "stdafx.h"
#include "building_info.hpp"
#include "../../bengine/btabs.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../global_assets/game_mode.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../raws/defs/reaction_t.hpp"
#include "../../raws/reactions.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"

namespace systems {
	namespace building_info {
		int selected_building;
		int last_building = -1;

		static const std::string win_building_info = std::string(ICON_FA_BUILDING_O) + " Building Info";
		static const std::string tab_info = std::string(ICON_FA_INFO_CIRCLE) + " Information";
		static const std::string tab_work = std::string(ICON_FA_BRIEFCASE) + " Possible Work";
		static const std::string tab_available = std::string(ICON_FA_INDUSTRY) + " Available Work";

		char building_name_buffer[254];

		static void render_base_info()
		{
			const auto be = bengine::entity(selected_building);
			if (!be) return;
			const auto bi = be->component<building_t>();
			if (!bi) return;
			const auto building_def = get_building_def(bi->tag);
			if (!building_def) return;
			const auto building_name = be->component<name_t>();
			if (!building_name) return;

			if (building_def)
			{
				ImGui::SameLine();
				ImGui::BeginChild("info_panel", ImVec2(300, 300));
				ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s", building_name->first_name);
				ImGui::Text("Rename Building: "); ImGui::SameLine();
				ImGui::InputText("## Building Name", building_name_buffer, 254);
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

		static void render_possible()
		{
			const auto be = bengine::entity(selected_building);
			if (!be) return;
			const auto bi = be->component<building_t>();
			if (!bi) return;
			const auto building_def = get_building_def(bi->tag);
			if (!building_def) return;

			ImGui::Text("This building provides the following work reactions:");

			each_reaction([&building_def] (const auto name, const auto reaction)
			{
				if (reaction->workshop == building_def->tag)
				{
					ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s", reaction->name.c_str());
					std::string inputs;
					for (const auto &i : reaction->inputs)
					{
						const auto item_def = get_item_def(i.tag);
						if (item_def)
						{
							inputs += item_def->name + " ";
						}
					}
					if (reaction->power_drain > 0) inputs += " Drains " + std::to_string(reaction->power_drain) + " power.";
					ImGui::Text("Inputs: %s", inputs.c_str());

					std::string outputs;
					for (const auto &o : reaction->outputs)
					{
						const auto item_def = get_item_def(o.first);
						if (item_def)
						{
							outputs += item_def->name + " ";
						}
					}
					ImGui::Text("Outputs: %s", outputs.c_str());
				}
			});
		}

		static bengine::btabs_t bi_tabs{
			{
				bengine::btab_t{ tab_info, render_base_info },
				bengine::btab_t{ tab_work, render_possible },
			}
		};

		static bool show_window = true;

		void run(const double &duration_ms)
		{
			if (last_building != selected_building)
			{
				last_building = selected_building;
				auto be = bengine::entity(selected_building);
				auto bn = be->component<name_t>();
				strncpy(building_name_buffer, bn->first_name.c_str(), 253);
			}

			bengine::begin_info_window(win_building_info, &show_window);
			bengine::render_btab_bar(bi_tabs);
			ImGui::End();

			if (!show_window) {
				game_master_mode = PLAY;
				show_window = true;
				auto be = bengine::entity(selected_building);
				auto bn = be->component<name_t>();
				bn->first_name = std::string(building_name_buffer);
			}
		}
	}
}
