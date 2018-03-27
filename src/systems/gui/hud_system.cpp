#include "stdafx.h"
#include "hud_system.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../global_assets/game_camera.hpp"
#include "../../render_engine/render_engine.hpp"
#include "../../bengine/main_window.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../global_assets/game_calendar.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../keydamper.hpp"
#include "../../render_engine/design_render.hpp"
#include "../../bengine/analytics.hpp"
#include <sstream>
#include "../../planet/region/region.hpp"
#include "../ai/settler/ai_work_template.hpp"
#include "../../main_loops/main_menu.hpp"

namespace systems {
    namespace hud {

        const static std::string menu_main = std::string(ICON_FA_ROCKET) + " Nox Futura";
        const static std::string menu_main_play = std::string(ICON_FA_PLAY) + " Play";
        const static std::string menu_main_quit = std::string(ICON_FA_FLOPPY_O) + " Save and Quit";
        const static std::string menu_design = std::string(ICON_FA_CUBES) + " Design";
        const static std::string menu_design_mining = std::string(ICON_FA_DIAMOND) + " Mining";
        const static std::string menu_design_building = std::string(ICON_FA_HOME) + " Building";
        const static std::string menu_design_tree_cutting = std::string(ICON_FA_TREE) + " Tree Cutting";
        const static std::string menu_design_guard = std::string(ICON_FA_USER_CIRCLE) + " Guard Posts";
        const static std::string menu_design_stockpiles = std::string(ICON_FA_AREA_CHART) + " Stockpiles";
        const static std::string menu_design_harvest = std::string(ICON_FA_SHOPPING_BASKET) + " Harvesting";
        const static std::string menu_design_architecture = std::string(ICON_FA_BUILDING) + " Architecture";
        const static std::string menu_units = std::string(ICON_FA_USERS) + " Units";
        const static std::string menu_units_units = std::string(ICON_FA_USERS) + " Units";
		const static std::string menu_units_jobcenter = std::string(ICON_FA_BRIEFCASE) + " Job Assignment";
		const static std::string menu_units_workflow = std::string(ICON_FA_WRENCH) + " Workflow Management";
        const static std::string menu_units_civs = std::string(ICON_FA_HOME) + " Other Civilizations";
        const static std::string menu_units_standing_orders = std::string(ICON_FA_LIST) + " Standing Orders";
		const static std::string menu_display = std::string(ICON_FA_CAMERA) + " Display";
		const static std::string menu_camera_view = std::string(ICON_FA_VIDEO_CAMERA) + " Camera Mode (TAB)";
		const static std::string menu_camera_perspective = std::string(ICON_FA_CUBE) + " Toggle Perspective (^TAB)";
		const static std::string menu_camera_ascii = std::string(ICON_FA_FONT) + " Toggle ASCII Mode (Shift+TAB)";
		const static std::string menu_camera_look = std::string(ICON_FA_EYE) + " Look";

		static void change_camera_view(const game_camera_mode_t new_mode)
		{
			camera->camera_mode = new_mode;
			render::camera_moved = true;
			render::models_changed = true;
			switch (new_mode)
			{
			case game_camera_mode_t::TOP_DOWN: bengine::analytics::on_event("game", "camera_mode", "Top-Down"); break;
			case game_camera_mode_t::FRONT: bengine::analytics::on_event("game", "camera_mode", "Front"); break;
			case game_camera_mode_t::DIAGONAL_LOOK_NW: bengine::analytics::on_event("game", "camera_mode", "Diagonal NW"); break;
			case game_camera_mode_t::DIAGONAL_LOOK_NE: bengine::analytics::on_event("game", "camera_mode", "Diagonal NE"); break;
			case game_camera_mode_t::DIAGONAL_LOOK_SW: bengine::analytics::on_event("game", "camera_mode", "Diagonal SW"); break;
			case game_camera_mode_t::DIAGONAL_LOOK_SE: bengine::analytics::on_event("game", "camera_mode", "Diagonal SE"); break;
			}
			
		}

		struct design_mode_t
		{
			std::string label;
			std::string shortcut;
			game_design_mode_t target;
		};

		static const std::vector<design_mode_t> DESIGN_MODES{
			design_mode_t{ menu_design_architecture, "^A", ARCHITECTURE },
			design_mode_t{ menu_design_building, "^B", BUILDING },
			design_mode_t{ menu_design_guard, "^G", GUARDPOINTS },
			design_mode_t{ menu_design_harvest, "^H", HARVEST },
			design_mode_t{ menu_design_mining, "^M", DIGGING },
			design_mode_t{ menu_design_stockpiles, "^S", STOCKPILES },
			design_mode_t{ menu_design_tree_cutting, "^T", CHOPPING },
		};

		static void add_design_elements()
		{
			for (const auto &mode : DESIGN_MODES)
			{
				if (ImGui::MenuItem(mode.label.c_str(), mode.shortcut.c_str()))
				{
					game_master_mode = DESIGN;
					pause_mode = PAUSED;
					game_design_mode = mode.target;
					render::mode_change = true;
				}
			}
		}

		static void normal_menu()
		{
			// Display cash
			fmt::MemoryWriter cash_ss;
			cash_ss << " Cash: " << designations->current_cash << " Mcr";
			fmt::MemoryWriter power_ss;
			power_ss << " Power: " << designations->current_power << "/" << designations->total_capacity << " ";
			fmt::MemoryWriter pause_ss;
			if (pause_mode) {
				pause_ss << "*PAUSED* (SPACE to unpause, . for single-step) ";
			}
			const std::string cash_str = pause_ss.str() + std::string(ICON_FA_CALENDAR) + std::string(" ") + calendar->get_date_time() + std::string(" ") + std::string(ICON_FA_LIGHTBULB_O) + power_ss.str() + std::string(ICON_FA_MONEY) + cash_ss.str();

			// Setup the main menu
			ImGui::BeginMainMenuBar();
			if (ImGui::BeginMenu(menu_main.c_str())) {
				if (ImGui::MenuItem(menu_main_play.c_str())) {
					game_master_mode = PLAY;
				}
				if (ImGui::MenuItem(menu_camera_look.c_str(), "k"))
				{
					game_master_mode = LOOK_MODE;
				}
				if (ImGui::BeginMenu(menu_display.c_str())) {
					if (!camera->ascii_mode) {
						if (ImGui::BeginMenu(menu_camera_view.c_str())) {
							if (ImGui::MenuItem("Top-Down")) change_camera_view(game_camera_mode_t::TOP_DOWN);
							if (ImGui::MenuItem("Front")) change_camera_view(game_camera_mode_t::FRONT);
							if (ImGui::MenuItem("Diagonal - Look NW")) change_camera_view(game_camera_mode_t::DIAGONAL_LOOK_NW);
							if (ImGui::MenuItem("Diagonal - Look NE")) change_camera_view(game_camera_mode_t::DIAGONAL_LOOK_NE);
							if (ImGui::MenuItem("Diagonal - Look SW")) change_camera_view(game_camera_mode_t::DIAGONAL_LOOK_SW);
							if (ImGui::MenuItem("Diagonal - Look SE")) change_camera_view(game_camera_mode_t::DIAGONAL_LOOK_SE);
							ImGui::EndMenu();
						}
						if (ImGui::MenuItem(menu_camera_perspective.c_str()))
						{
							camera->perspective = !camera->perspective;
							render::camera_moved = true;
							render::models_changed = true;
						}
					}
					if (ImGui::MenuItem(menu_camera_ascii.c_str())) {
						camera->ascii_mode = !camera->ascii_mode;
						bengine::analytics::on_event("game", "renderMode", camera->ascii_mode ? "ASCII" : "3D");
						render::camera_moved = true;
						render::models_changed = true;
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem(menu_main_quit.c_str())) {
					region::save_current_region();
					std::unique_ptr<std::ofstream> lbfile = std::make_unique<std::ofstream>(save_filename(), std::ios::out | std::ios::binary);
					bengine::ecs_save(lbfile);
					bengine::main_func = main_menu::tick;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(menu_design.c_str())) {
				add_design_elements();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(menu_units.c_str())) {
				if (ImGui::MenuItem(menu_units_units.c_str(), "U")) {
					game_master_mode = UNITS;
					pause_mode = PAUSED;
				}
				if (ImGui::MenuItem(menu_units_workflow.c_str(), "W")) {
					game_master_mode = WORKFLOW;
					pause_mode = PAUSED;
				}
				if (ImGui::MenuItem(menu_units_civs.c_str(), "C")) {
					game_master_mode = CIVS;
					pause_mode = PAUSED;
				}
				if (ImGui::MenuItem(menu_units_standing_orders.c_str())) {
					/*game_master_mode = STANDING_ORDERS;
					pause_mode = PAUSED;
					emit_deferred(map_dirty_message{});*/
				}
				if (ImGui::MenuItem(menu_units_jobcenter.c_str(), "J")) {
					game_master_mode = JOB_CENTER_MODE;
					pause_mode = PAUSED;
				}
				ImGui::EndMenu();
			}

			const auto status_size = ImGui::CalcTextSize(cash_str.c_str());
			ImGui::SameLine(ImGui::GetWindowWidth() - (status_size.x + 10));
			ImGui::Text("%s", cash_str.c_str());
			ImGui::EndMainMenuBar();
		}

		static void design_menu()
		{
			ImGui::BeginMainMenuBar();
			ImGui::Text("%s", "Design Mode");

			for (const auto &mode : DESIGN_MODES)
			{
				const auto modecat = mode.label + std::string(" (") + mode.shortcut + std::string(")");
				if (ImGui::BeginMenu(modecat.c_str()))
				{
					game_master_mode = DESIGN;
					pause_mode = PAUSED;
					game_design_mode = mode.target;
					render::mode_change = true;
					ImGui::EndMenu();
				}
			}

			ImGui::EndMainMenuBar();
		}

        void run(const double &duration_ms) {
			if (game_master_mode != DESIGN) {
				normal_menu();

				// Keyboard shortcuts
				if (game_master_mode == LOOK_MODE)
				{
					ImGui::Begin("Look Mode");
					ImGui::Text("%s", "Arrow keys to move cursor, shift to move camera, escape to return to play.");
					ImGui::End();
					if (is_key_down(GLFW_KEY_ESCAPE)) game_master_mode = PLAY;
				}

				if (game_master_mode == PLAY) {
					const auto pressed_control = glfwGetKey(bengine::main_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(bengine::main_window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
					if (is_key_down(GLFW_KEY_U, false)) game_master_mode = UNITS;
					if (is_key_down(GLFW_KEY_C, false)) game_master_mode = CIVS;
					if (!pressed_control && is_key_down(GLFW_KEY_W, false)) game_master_mode = WORKFLOW;
					if (is_key_down(GLFW_KEY_J, false)) game_master_mode = JOB_CENTER_MODE;
					if (pressed_control && is_key_down(GLFW_KEY_W, false)) game_master_mode = WISHMODE;
					if (is_key_down(GLFW_KEY_APOSTROPHE)) render::depth_test_render = !render::depth_test_render;
					if (is_key_down(GLFW_KEY_K)) game_master_mode = LOOK_MODE;
				}
			} else
			{
				design_menu();
			}
		}
    }
}
