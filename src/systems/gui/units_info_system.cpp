#include "stdafx.h"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../render_engine/camera.hpp"
#include "../../render_engine/vox/renderables.hpp"
#include "../../bengine/btabs.hpp"
#include "../../bengine/main_window.hpp"

namespace systems {
	namespace units_ui {

		const static std::string win_units = std::string(ICON_FA_USERS) + " Units";
		const static std::string win_settler_list = std::string(ICON_FA_USERS) + " Settlers";
		const static std::string win_wildlife_list = std::string(ICON_FA_PAW) + " Wildlife";
		const static std::string win_natives_list = std::string(ICON_FA_USER_SECRET) + " Other Sentients";
		const static std::string btn_goto = std::string(ICON_FA_MAP_MARKER) + " Go To";
		const static std::string btn_goto_creature = std::string(ICON_FA_MAP_MARKER) + " Go To Creature";
		const static std::string btn_goto_native = std::string(ICON_FA_MAP_MARKER) + " Go To NPC";
		const static std::string btn_rogue = std::string(ICON_FA_USER) + " Control";
		const static std::string btn_close = std::string(ICON_FA_TIMES) + " Close";
		static bool show_window = true;

		int selected_settler = 0;
		int current_settler = 0;
		int current_critter = 0;
		int current_native = 0;

		static bool settler_headings_first_run = true;
		static std::vector<bengine::table_heading_t> settler_headings{
			{ "Settler Name", -1.0f }, { "Profession", -1.0f}, {"Status", -1.0f}, {"Task", -1.0f}, { "Options", -1.0f }
		};

		static inline void render_settlers() {
			using namespace bengine;

			bengine::begin_table(settler_headings_first_run, settler_headings, "settler_list_grid", true);

			bool zebra = true;
			each<settler_ai_t, name_t, game_stats_t, species_t, health_t>([&zebra](entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats, species_t &species, health_t &h) {
				bengine::zebra_row(zebra);
				const std::string gender = (species.gender == MALE) ? std::string(ICON_FA_MALE) : std::string(ICON_FA_FEMALE);
				const std::string dname = name.first_name + std::string(" ") + name.last_name;
				const std::string profession = stats.profession_tag;
				const std::string task = ai.job_status;
				std::string hp = std::string("HP:") + std::to_string(h.current_hitpoints) + std::string("/") + std::to_string(h.max_hitpoints);
				if (h.blind) hp += std::string(" Blind.");
				if (h.no_grasp) hp += std::string(" Cannot grasp.");
				if (h.slow) hp += std::string(" Slow.");
				if (h.unconscious) hp += std::string(" Unsconscious.");
				const float hp_percent = static_cast<float>(h.current_hitpoints) / static_cast<float>(h.max_hitpoints);
				ImVec4 health_color{ 0.0f, 1.0f, 0.0f, 1.0f };
				if (hp_percent < 0.2f) {
					health_color.x = 1.0f;
					health_color.y = 0.0f;
				}
				else if (hp_percent < 0.75f) {
					health_color.x = 1.0f;
					health_color.y = 1.0f;
				}

				bengine::begin_zebra_col(zebra);
				ImGui::Text("%s %s", gender.c_str(), dname.c_str());
				bengine::end_zebra_col();
				bengine::begin_zebra_col(zebra);
				ImGui::Text("%s", profession.c_str());
				bengine::end_zebra_col();
				bengine::begin_zebra_col(zebra);
				ImGui::TextColored(health_color, "%s", hp.c_str());
				bengine::end_zebra_col();
				bengine::begin_zebra_col(zebra);
				ImGui::Text("%s", task.c_str());
				bengine::end_zebra_col();
				bengine::begin_zebra_col(zebra);

				const std::string btn_view = std::string(ICON_FA_SEARCH_PLUS) + " Go To##" + std::to_string(e.id);
				if (ImGui::SmallButton(btn_view.c_str())) {
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
				if (ImGui::SmallButton(btn_roguemode.c_str())) {
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
				const std::string btn_viewmode = std::string(ICON_FA_USER_CIRCLE) + std::string(" View##") + std::to_string(e.id);
				if (ImGui::SmallButton(btn_viewmode.c_str())) {
					game_master_mode = SETTLER;
					selected_settler = e.id;
				}

				ImGui::SameLine();
				const std::string btn_followmode = std::string(ICON_FA_VIDEO_CAMERA) + std::string(" Follow##") + std::to_string(e.id);
				if (ImGui::SmallButton(btn_followmode.c_str())) {
					camera->following = e.id;
					game_master_mode = PLAY;
				}

				bengine::end_zebra_col();
			});
			bengine::end_table();
		}

		static bool critter_headings_first_run = true;
		static std::vector<bengine::table_heading_t> critter_headings{
			{ "Creature", 150.0f },{ "Options", -1.0f }
		};

		static inline void render_creatures() {
			using namespace bengine;

			bengine::begin_table(critter_headings_first_run, critter_headings, "critter_list_grid");

			bool zebra = true;
			each<grazer_ai, name_t>([&zebra](entity_t &e, grazer_ai &ai, name_t &name) {
				bengine::zebra_row(zebra);
				bengine::begin_zebra_col(zebra);
				const std::string critter_name = name.first_name + std::string(" ") + name.last_name;
				ImGui::Text("%s", critter_name.c_str());
				bengine::end_zebra_col();

				bengine::begin_zebra_col(zebra);
				const std::string critter_goto = btn_goto_creature + std::string("##") + std::to_string(e.id);
				if (ImGui::SmallButton(critter_goto.c_str())) {
					auto pos = e.component<position_t>();
					camera_position->region_x = pos->x;
					camera_position->region_y = pos->y;
					camera_position->region_z = pos->z;
					game_master_mode = PLAY;
					render::camera_moved = true;
					render::models_changed = true;
				}
				bengine::end_zebra_col();
			});
			bengine::end_table();
		}

		static bool native_headings_first_run = true;
		static std::vector<bengine::table_heading_t> native_headings{
			{ "NPC Name", 150.0f }, { "Options", -1.0f }
		};

		static inline void render_natives() {
			using namespace bengine;

			bengine::begin_table(native_headings_first_run, native_headings, "npc_list_grid");

			bool zebra = true;
			each<sentient_ai, name_t>([&zebra](entity_t &e, sentient_ai &ai, name_t &name) {
				bengine::zebra_row(zebra);

				bengine::begin_zebra_col(zebra);
				const std::string npc_name = name.first_name + std::string(" ") + name.last_name;
				ImGui::Text("%s", npc_name.c_str());
				bengine::end_zebra_col();

				bengine::begin_zebra_col(zebra);
				const std::string critter_goto = btn_goto_creature + std::string("##") + std::to_string(e.id);
				if (ImGui::SmallButton(critter_goto.c_str())) {
					auto pos = e.component<position_t>();
					camera_position->region_x = pos->x;
					camera_position->region_y = pos->y;
					camera_position->region_z = pos->z;
					game_master_mode = PLAY;
					render::camera_moved = true;
					render::models_changed = true;
					show_window = false;
				}
				bengine::end_zebra_col();
			});
			bengine::end_table();
		}

		static bengine::btabs_t unit_tabs{
			{
				bengine::btab_t{ win_settler_list, render_settlers},
				bengine::btab_t{ win_wildlife_list, render_creatures },
				bengine::btab_t{ win_natives_list, render_natives },
			}
		};

		void run(const double &duration_ms) {
			bengine::begin_info_window(win_units, &show_window);
			bengine::render_btab_bar(unit_tabs);
			ImGui::End();

			if (!show_window) {
				game_master_mode = PLAY;
				show_window = true;
			}

		}
	}
}
