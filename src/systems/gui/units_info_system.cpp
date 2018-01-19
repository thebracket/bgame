#include "stdafx.h"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../../bengine/imgui_tabs.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/name.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/species.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../render_engine/camera.hpp"
#include "../../render_engine/vox/renderables.hpp"
#include "../../stdafx.h"

namespace systems {
	namespace units_ui {

		const std::string win_units = std::string(ICON_FA_USERS) + " Units";
		const std::string win_settler_list = std::string(ICON_FA_USERS) + " Settlers";
		const std::string win_wildlife_list = std::string(ICON_FA_PAW) + " Wildlife";
		const std::string win_natives_list = std::string(ICON_FA_USER_SECRET) + " Other Sentients";
		const std::string btn_goto = std::string(ICON_FA_MAP_MARKER) + " Go To";
		const std::string btn_goto_creature = std::string(ICON_FA_MAP_MARKER) + " Go To Creature";
		const std::string btn_goto_native = std::string(ICON_FA_MAP_MARKER) + " Go To NPC";
		const std::string btn_rogue = std::string(ICON_FA_USER) + " Control";
		const std::string btn_close = std::string(ICON_FA_TIMES) + " Close";

		int selected_settler = 0;
		int current_settler = 0;
		int current_critter = 0;
		int current_native = 0;

		void render_settlers() {
			using namespace bengine;

			ImGui::Columns(4, "settler_list_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4( 1.0f, 1.0f, 0.0f, 1.0f ), "%s", "Settler Name"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Profession"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Task"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Options"); ImGui::NextColumn();
			ImGui::Separator();

			each<settler_ai_t, name_t, game_stats_t, species_t>([](entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats, species_t &species) {
				const std::string gender = (species.gender == MALE) ? std::string(ICON_FA_MALE) : std::string(ICON_FA_FEMALE);
				const std::string dname = name.first_name + std::string(" ") + name.last_name;
				const std::string profession = stats.profession_tag;
				const std::string task = ai.job_status;

				ImGui::Text("%s %s", gender.c_str(), dname.c_str());
				ImGui::NextColumn();
				ImGui::Text("%s", profession.c_str());
				ImGui::NextColumn();
				ImGui::Text("%s", task.c_str());
				ImGui::NextColumn();

				const std::string btn_view = std::string(ICON_FA_USER_CIRCLE) + " View##" + std::to_string(e.id);
				if (ImGui::Button(btn_view.c_str())) {
					auto pos = e.component<position_t>();
					camera_position->region_x = pos->x;
					camera_position->region_y = pos->y;
					camera_position->region_z = pos->z;
					game_master_mode = PLAY;
					render::camera_moved = true;
					render::models_changed = true;
				}

				ImGui::SameLine();
				const std::string btn_roguemode = btn_rogue + std::string("##") + std::to_string(e.id);
				if (ImGui::Button(btn_roguemode.c_str())) {
					auto pos = e.component<position_t>();
					if (pos) {
						camera_position->region_x = pos->x;
						camera_position->region_y = pos->y;
						camera_position->region_z = pos->z;
					}

					auto ai = e.component<settler_ai_t>();
					if (ai) {
						ai->job_type_major = JOB_IDLE;
					}
					game_master_mode = ROGUE;
					selected_settler = e.id;
				}

				ImGui::SameLine();
				const std::string btn_viewmode = btn_view + std::string("##") + std::to_string(e.id);
				if (ImGui::Button(btn_viewmode.c_str())) {
					game_master_mode = SETTLER;
					selected_settler = e.id;
				}
				
				ImGui::NextColumn();
				ImGui::Separator();
			});						
		}

		void render_creatures() {
			using namespace bengine;

			ImGui::Columns(2, "critter_list_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Creature"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Options"); ImGui::NextColumn();
			ImGui::Separator();

			each<grazer_ai, name_t>([](entity_t &e, grazer_ai &ai, name_t &name) {
				const std::string critter_name = name.first_name + std::string(" ") + name.last_name;
				ImGui::Text("%s", critter_name.c_str());
				ImGui::NextColumn();
				ImGui::Separator();

				const std::string critter_goto = btn_goto_creature + std::string("##") + std::to_string(e.id);
				if (ImGui::Button(critter_goto.c_str())) {
					auto pos = e.component<position_t>();
					camera_position->region_x = pos->x;
					camera_position->region_y = pos->y;
					camera_position->region_z = pos->z;
					game_master_mode = PLAY;
					render::camera_moved = true;
					render::models_changed = true;
				}
				ImGui::NextColumn();
				ImGui::Separator();
			});

			ImGui::Columns(1);
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
			if (ImGui::Button(std::string(btn_goto_native + std::string(" ")).c_str())) {
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
			ImGui::Begin(win_units.c_str(), nullptr, ImGuiWindowFlags_NoCollapse);

			ImGui::SameLine();
			if (ImGui::Button(btn_close.c_str())) {
				game_master_mode = PLAY;
			}

			ImGui::BeginTabBar("Units#left_tab_bar");
			ImGui::DrawTabsBackground();
			if (ImGui::AddTab(win_settler_list.c_str())) {
				render_settlers();
			}
			if (ImGui::AddTab(win_wildlife_list.c_str())) {
				render_creatures();
			}
			if (ImGui::AddTab(win_natives_list.c_str())) {
				render_natives();
			}
			ImGui::EndTabBar();

			ImGui::End();
		}
	}
}