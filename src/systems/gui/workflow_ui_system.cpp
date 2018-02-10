#include "workflow_ui_system.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../raws/reactions.hpp"
#include "../../raws/defs/reaction_t.hpp"
#include "../helpers/inventory_assistant.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/building_designations.hpp"

namespace systems {
	namespace workflow_ui {
		const std::string win_workflow = std::string(ICON_FA_WRENCH) + " Workflow Management";
		const std::string btn_build = std::string(ICON_FA_PLUS) + " Queue Job";
		const std::string btn_close = std::string(ICON_FA_TIMES) + " Close";
		int selected_work_item = 0;
		int selected_build_item = 0;

		void run(const double &duration_ms) {
			std::vector<std::string> worklist;
			for (const std::pair<uint8_t, std::string> &order : building_designations->build_orders) {
				auto finder = get_reaction_def(order.second);
				if (finder != nullptr) {
					worklist.emplace_back(std::to_string(order.first) + std::string(" ") + finder->name);
				}
			}
			std::vector<const char *> work_listbox_items(worklist.size());
			for (int i = 0; i<worklist.size(); ++i) {
				work_listbox_items[i] = worklist[i].c_str();
			}

			ImGui::Begin(win_workflow.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
			ImGui::ListBox("Queued Jobs", &selected_work_item, &work_listbox_items[0], work_listbox_items.size(), 10);

			std::vector<std::string> buildlist;
			auto available_reactions = inventory::get_available_reactions();
			for (const auto &reaction : available_reactions) {
				buildlist.emplace_back(reaction.second);
			}
			std::vector<const char *> buildable_listbox_items(buildlist.size());
			for (int i = 0; i<buildlist.size(); ++i) {
				buildable_listbox_items[i] = buildlist[i].c_str();
			}

			ImGui::ListBox("Available Jobs", &selected_build_item, &buildable_listbox_items[0], buildable_listbox_items.size(), 10);
			if (ImGui::Button(btn_build.c_str())) {
				const std::string tag = available_reactions[selected_build_item].first;
				bool found = false;
				for (auto &order : building_designations->build_orders) {
					if (order.second == tag) {
						++order.first;
						found = true;
					}
				}
				if (!found) building_designations->build_orders.push_back(std::make_pair(1, tag));
			}
			ImGui::SameLine();
			if (ImGui::Button(btn_close.c_str())) {
				game_master_mode = PLAY;
			}

			ImGui::End();
		}
	}
}
