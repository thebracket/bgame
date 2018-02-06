#include "civ_ui_system.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/game_planet.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"

namespace systems {
	namespace civ_ui {

		int selected_civ = 0;
		int negotiating_civ = 0;
		const std::string win_civs_list = std::string(ICON_FA_HOME) + " Other Civilizations";

		void render_civ_list() {
			std::vector<std::pair<std::size_t, std::string>> civs;
			std::size_t civ_id = 0;
			for (civ_t &civ : planet.civs.civs) {
				if (civ.met_cordex) {
					std::string line = civ.name + std::string(" ");
					if (civ.cordex_feelings < 0) {
						line += "(WAR)";
					}
					else if (civ.cordex_feelings == 0) {
						line += "(NEUTRAL)";
					}
					else {
						line += "(FRIENDLY)";
					}
					civs.emplace_back(std::make_pair(civ_id, line));
				}
				++civ_id;
			}
			std::vector<const char *> civ_listbox_items;
			civ_listbox_items.resize(civs.size());

			for (int i = 0; i<civs.size(); ++i) {
				civ_listbox_items[i] = civs[i].second.c_str();
			}
			ImGui::Begin(win_civs_list.c_str(), nullptr, ImVec2{ 600,400 }, 0.9f, ImGuiWindowFlags_NoCollapse);
			ImGui::PushItemWidth(-1);
			ImGui::ListBox("## ", &selected_civ, &civ_listbox_items[0], civs.size(), 10);

			if (ImGui::Button("Negotiate")) {
				negotiating_civ = static_cast<int>(civs[selected_civ].first);
				game_master_mode = CIV_NEGOTIATE;
			}

			ImGui::SameLine();
			if (ImGui::Button("Close")) {
				game_master_mode = PLAY;
			}

			ImGui::End();
		}

		void render_civ_negotiate() {
			const std::string title = std::string("Negotiations with ") + planet.civs.civs[negotiating_civ].name;
			const std::string salutation = std::string("Greeting, Cordex. We are The ") + planet.civs.civs[negotiating_civ].name;
			std::string feelings;
			ImVec4 feelings_color{ 1.0f, 1.0f, 1.0f, 1.0f };
			bool show_bribe = false;
			bool show_war = false;

			if (planet.civs.civs[negotiating_civ].cordex_feelings < 0) {
				feelings = "You have invaded our lands, harassed our people, and must die.";
				feelings_color = ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f };
				show_bribe = true;
			}
			else if (planet.civs.civs[negotiating_civ].cordex_feelings == 0) {
				feelings = "You have occupied our lands, but perhaps we can work something out?";
				feelings_color = ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f };
				show_bribe = true;
				show_war = true;
			}
			else {
				feelings = "We welcome your presence here, and look forward to working with you.";
				feelings_color = ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f };
				show_war = true;
			}

			ImGui::Begin(title.c_str());
			ImGui::Text("%s", salutation.c_str());
			ImGui::TextColored(feelings_color, "%s", feelings.c_str());
			if (show_bribe) {
				if (ImGui::Button("Maybe 50 Mcr will make you feel better?")) {
					game_master_mode = PLAY;
					planet.civs.civs[negotiating_civ].cordex_feelings = 0;
					designations->current_cash -= 50;
				}
			}
			if (show_war) {
				if (ImGui::Button("I hope that you all die horribly in a fire!")) {
					game_master_mode = PLAY;
					planet.civs.civs[negotiating_civ].cordex_feelings = -1;
				}
			}
			if (ImGui::Button("End Negotiation")) {
				game_master_mode = PLAY;
			}
			ImGui::End();
		}

		void run(const double &duration_ms) {

			if (game_master_mode == CIVS) {
				render_civ_list();
			}
			else if (game_master_mode == CIV_NEGOTIATE) {
				render_civ_negotiate();
			}
		}
	}
}