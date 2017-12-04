#pragma once

#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../../bengine/ecs.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/name.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/species.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../stdafx.h"

namespace systems {
	namespace units_ui {

		const std::string win_units = std::string(ICON_FA_USERS) + " Units";
		const std::string win_settler_list = std::string(ICON_FA_USERS) + " Settlers";
		const std::string win_wildlife_list = std::string(ICON_FA_PAW) + " Wildlife";
		const std::string win_natives_list = std::string(ICON_FA_USER_SECRET) + " Natives";
		const std::string btn_goto = std::string(ICON_FA_MAP_MARKER) + " Go To";
		const std::string btn_goto_creature = std::string(ICON_FA_MAP_MARKER) + " Go To Creature";
		const std::string btn_rogue = std::string(ICON_FA_USER) + " Control (Rogue Mode)";
		const std::string btn_close = std::string(ICON_FA_TIMES) + " Close";

		int selected_settler = 0;
		int current_settler = 0;
		int current_critter = 0;
		int current_native = 0;

		void render_settlers() {
			using namespace bengine;

			// Build settler data structures
			std::vector<std::pair<std::size_t, std::string>> settlers;
			each<settler_ai_t, name_t, game_stats_t, species_t>([&settlers](entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats, species_t &species) {
				const std::string gender = (species.gender == MALE) ? std::string(ICON_FA_MALE) : std::string(ICON_FA_FEMALE);
				const std::string display = gender + std::string(" ") + name.first_name + std::string(" ") + name.last_name + std::string(" (") + stats.profession_tag + std::string(") : ") + ai.job_status;
				settlers.emplace_back(std::make_pair(e.id, display));
			});
			std::vector<const char *> settler_listbox_items;
			settler_listbox_items.resize(settlers.size());
			for (int i = 0; i<settlers.size(); ++i) {
				settler_listbox_items[i] = settlers[i].second.c_str();
			}

			// Render the settlers window
			//ImGui::Begin(win_settler_list.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::PushItemWidth(-1);
			ImGui::ListBox("## Settlers", &current_settler, &settler_listbox_items[0], settlers.size(), 10);
			const std::string btn_view = std::string(ICON_FA_USER_CIRCLE) + " View";
			if (ImGui::Button(btn_view.c_str())) {
				game_master_mode = SETTLER;
				selected_settler = static_cast<int>(settlers[current_settler].first);
			}
			ImGui::SameLine();
			if (ImGui::Button(btn_goto.c_str())) {
				selected_settler = static_cast<int>(settlers[current_settler].first);
				auto the_settler = entity(selected_settler);
				if (the_settler) {
					auto pos = the_settler->component<position_t>();
					camera_position->region_x = pos->x;
					camera_position->region_y = pos->y;
					camera_position->region_z = pos->z;
				}
				game_master_mode = PLAY;
			}
			ImGui::SameLine();
			if (ImGui::Button(btn_rogue.c_str())) {
				selected_settler = static_cast<int>(settlers[current_settler].first);
				auto the_settler = entity(selected_settler);
				if (the_settler) {
					auto pos = the_settler->component<position_t>();
					if (pos) {
						camera_position->region_x = pos->x;
						camera_position->region_y = pos->y;
						camera_position->region_z = pos->z;
					}

					auto ai = the_settler->component<settler_ai_t>();
					if (ai) {
						ai->job_type_major = JOB_IDLE;
					}
				}
				game_master_mode = ROGUE;
			}
			ImGui::SameLine();
			if (ImGui::Button(btn_close.c_str())) {
				game_master_mode = PLAY;
			}
		}

		void render_creatures() {
			using namespace bengine;

			std::vector<std::pair<std::size_t, std::string>> critters;
			each<grazer_ai, name_t>([&critters](entity_t &e, grazer_ai &ai, name_t &name) {
				critters.emplace_back(std::make_pair(e.id, name.first_name + std::string(" ") + name.last_name));
			});
			std::vector<const char *> creature_listbox_items;
			creature_listbox_items.resize(critters.size());
			for (int i = 0; i<critters.size(); ++i) {
				creature_listbox_items[i] = critters[i].second.c_str();
			}
			//ImGui::Begin(win_wildlife_list.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::PushItemWidth(-1);
			ImGui::ListBox("## Critters", &current_critter, &creature_listbox_items[0], critters.size(), 10);
			if (ImGui::Button(btn_goto_creature.c_str())) {
				auto selected_critter = critters[current_settler].first;
				auto the_critter = entity(selected_critter);
				if (the_critter) {
					auto pos = the_critter->component<position_t>();
					camera_position->region_x = pos->x;
					camera_position->region_y = pos->y;
					camera_position->region_z = pos->z;
				}
				game_master_mode = PLAY;
			}
			ImGui::SameLine();
			if (ImGui::Button(btn_close.c_str())) {
				game_master_mode = PLAY;
			}
		}

		void render_natives() {
			using namespace bengine;
			std::vector<std::pair<std::size_t, std::string>> natives;
			each<sentient_ai, name_t>([&natives](entity_t &e, sentient_ai &ai, name_t &name) {
				natives.emplace_back(std::make_pair(e.id, name.first_name + std::string(" ") + name.last_name + std::string(" #") + std::to_string(e.id)));
			});
			std::vector<const char *> native_listbox_items;
			native_listbox_items.resize(natives.size());
			for (int i = 0; i<natives.size(); ++i) {
				native_listbox_items[i] = natives[i].second.c_str();
			}
			//ImGui::Begin(win_natives_list.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::PushItemWidth(-1);
			ImGui::ListBox("## Natives", &current_native, &native_listbox_items[0], natives.size(), 10);
			if (ImGui::Button(std::string(btn_goto + std::string(" ")).c_str())) {
				auto selected_critter = natives[current_native].first;
				auto the_critter = entity(selected_critter);
				if (the_critter) {
					auto pos = the_critter->component<position_t>();
					camera_position->region_x = pos->x;
					camera_position->region_y = pos->y;
					camera_position->region_z = pos->z;
				}
				game_master_mode = PLAY;
			}
			ImGui::SameLine();
			if (ImGui::Button("Close")) {
				game_master_mode = PLAY;
			}
		}

		void run(const double &duration_ms) {
			ImGui::Begin(win_units.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
			if (ImGui::CollapsingHeader(win_settler_list.c_str()), ImGuiTreeNodeFlags_DefaultOpen) {
				render_settlers();
			}
			if (ImGui::CollapsingHeader(win_wildlife_list.c_str())) {
				render_creatures();
			}
			if (ImGui::CollapsingHeader(win_natives_list.c_str())) {
				render_natives();
			}
			ImGui::End();
		}
	}
}