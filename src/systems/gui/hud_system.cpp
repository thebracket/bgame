#include "hud_system.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
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

namespace systems {
    namespace hud {

        const std::string menu_main = std::string(ICON_FA_ROCKET) + " Nox Futura";
        const std::string menu_main_play = std::string(ICON_FA_PLAY) + " Play";
        const std::string menu_main_quit = std::string(ICON_FA_WINDOW_CLOSE) + " Save and Quit";
        const std::string menu_design = std::string(ICON_FA_CUBES) + " Design";
        const std::string menu_design_mining = std::string(ICON_FA_DIAMOND) + " Mining";
        const std::string menu_design_building = std::string(ICON_FA_HOME) + " Building";
        const std::string menu_design_tree_cutting = std::string(ICON_FA_TREE) + " Tree Cutting";
        const std::string menu_design_guard = std::string(ICON_FA_USER_CIRCLE) + " Guard Posts";
        const std::string menu_design_stockpiles = std::string(ICON_FA_AREA_CHART) + " Stockpiles";
        const std::string menu_design_harvest = std::string(ICON_FA_SHOPPING_BASKET) + " Harvesting";
        const std::string menu_design_architecture = std::string(ICON_FA_BUILDING) + " Architecture";
        const std::string menu_units = std::string(ICON_FA_USERS) + " Units";
        const std::string menu_units_units = std::string(ICON_FA_USERS) + " Units";
		const std::string menu_units_jobcenter = std::string(ICON_FA_BRIEFCASE) + " Job Assignment";
		const std::string menu_units_workflow = std::string(ICON_FA_WRENCH) + " Workflow Management";
        const std::string menu_units_civs = std::string(ICON_FA_HOME) + " Other Civilizations";
        const std::string menu_units_standing_orders = std::string(ICON_FA_LIST) + " Standing Orders";
		const std::string menu_display = std::string(ICON_FA_CAMERA) + " Display";

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

        void run(const double &duration_ms) {
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
                    /*game_master_mode = PLAY;
                    emit_deferred(map_dirty_message{});
                    emit_deferred(recalculate_mining_message{});
                    world_changed=true;*/
                }
                if (ImGui::BeginMenu(menu_display.c_str())) {
					if (!camera->ascii_mode) {
						if (ImGui::MenuItem("Top-Down")) change_camera_view(game_camera_mode_t::TOP_DOWN);
						if (ImGui::MenuItem("Front")) change_camera_view(game_camera_mode_t::FRONT);
						if (ImGui::MenuItem("Diagonal - Look NW")) change_camera_view(game_camera_mode_t::DIAGONAL_LOOK_NW);
						if (ImGui::MenuItem("Diagonal - Look NE")) change_camera_view(game_camera_mode_t::DIAGONAL_LOOK_NE);
						if (ImGui::MenuItem("Diagonal - Look SW")) change_camera_view(game_camera_mode_t::DIAGONAL_LOOK_SW);
						if (ImGui::MenuItem("Diagonal - Look SE")) change_camera_view(game_camera_mode_t::DIAGONAL_LOOK_SE);
					}
					if (ImGui::MenuItem("Toggle ASCII")) {
                        camera->ascii_mode = !camera->ascii_mode;
						bengine::analytics::on_event("game", "renderMode", camera->ascii_mode ? "ASCII" : "3D");
						render::camera_moved = true;
						render::models_changed = true;
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem(menu_main_quit.c_str())) {
                    glfwSetWindowShouldClose(bengine::main_window, true);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu(menu_design.c_str())) {
                if (ImGui::MenuItem(menu_design_mining.c_str())) {
                    game_master_mode = DESIGN;
                    pause_mode = PAUSED;
                    game_design_mode = DIGGING;
					render::mode_change = true;
                }
                if (ImGui::MenuItem(menu_design_building.c_str())) {
                    game_master_mode = DESIGN;
                    pause_mode = PAUSED;
                    game_design_mode = BUILDING;
					render::mode_change = true;
                }
                if (ImGui::MenuItem(menu_design_tree_cutting.c_str())) {
                    game_master_mode = DESIGN;
                    pause_mode = PAUSED;
                    game_design_mode = CHOPPING;
                }
                if (ImGui::MenuItem(menu_design_guard.c_str())) {
                    game_master_mode = DESIGN;
                    pause_mode = PAUSED;
                    game_design_mode = GUARDPOINTS;
					render::mode_change = true;
                }
                if (ImGui::MenuItem(menu_design_stockpiles.c_str())) {
                    game_master_mode = DESIGN;
                    pause_mode = PAUSED;
                    game_design_mode = STOCKPILES;
					render::mode_change = true;
                }
                if (ImGui::MenuItem(menu_design_harvest.c_str())) {
                    game_master_mode = DESIGN;
                    pause_mode = PAUSED;
                    game_design_mode = HARVEST;
					render::mode_change = true;
                }
                if (ImGui::MenuItem(menu_design_architecture.c_str())) {
                    game_master_mode = DESIGN;
                    pause_mode = PAUSED;
                    game_design_mode = ARCHITECTURE;
					render::mode_change = true;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu(menu_units.c_str())) {
                if (ImGui::MenuItem(menu_units_units.c_str())) {
                    game_master_mode = UNITS;
                    pause_mode = PAUSED;
                }
                if (ImGui::MenuItem(menu_units_workflow.c_str())) {
                    game_master_mode = WORKFLOW;
                    pause_mode = PAUSED;
                }
                if (ImGui::MenuItem(menu_units_civs.c_str())) {
                    game_master_mode = CIVS;
                    pause_mode = PAUSED;
                }
                if (ImGui::MenuItem(menu_units_standing_orders.c_str())) {
                    /*game_master_mode = STANDING_ORDERS;
                    pause_mode = PAUSED;
                    emit_deferred(map_dirty_message{});*/
                }
				if (ImGui::MenuItem(menu_units_jobcenter.c_str())) {
					game_master_mode = JOB_CENTER_MODE;
					pause_mode = PAUSED;
				}
                ImGui::EndMenu();
            }

            auto status_size = ImGui::CalcTextSize(cash_str.c_str());
            ImGui::SameLine(ImGui::GetWindowWidth()-(status_size.x + 10));
            ImGui::Text("%s", cash_str.c_str());
            ImGui::EndMainMenuBar();

			// Keyboard shortcuts
			if (game_master_mode == PLAY) {
				if (is_key_down(GLFW_KEY_U, false)) game_master_mode = UNITS;
				if (is_key_down(GLFW_KEY_C, false)) game_master_mode = CIVS;
				if (is_key_down(GLFW_KEY_W, false)) game_master_mode = WORKFLOW;
				if (is_key_down(GLFW_KEY_J, false)) game_master_mode = JOB_CENTER_MODE;
				if (is_key_down(GLFW_KEY_W, false)) game_master_mode = WISHMODE;
				if (is_key_down(GLFW_KEY_APOSTROPHE)) render::depth_test_render = !render::depth_test_render;
			}
		}
    }
}