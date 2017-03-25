#include "hud_system.hpp"
#include "../../main/game_globals.hpp"
#include "../../messages/messages.hpp"
#include <sstream>
#include "imgui_helper.hpp"
#include "../../external/imgui-sfml/imgui-SFML.h"
#include "../../utils/gl/map_render.hpp"

constexpr int HUD_TERM = 3;
const std::string menu_main = std::string(ICON_FA_ROCKET) + " Black Future";
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
const std::string menu_units_workflow = std::string(ICON_FA_WRENCH) + " Workflow Management";
const std::string menu_units_civs = std::string(ICON_FA_HOME) + " Other Civilizations";
const std::string menu_units_standing_orders = std::string(ICON_FA_LIST) + " Standing Orders";

void hud_system::configure() {
    system_name = "HUD System";
}

void hud_system::update(const double ms) {

    // Display cash
    std::stringstream cash_ss;
	cash_ss << " Cash: " << designations->current_cash << " Mcr";
    std::stringstream power_ss;
    power_ss << " Power: " << designations->current_power << "/" << designations->total_capacity << " ";
    std::stringstream pause_ss;
    if (pause_mode) {
        pause_ss << "*PAUSED* (SPACE to unpause, . for single-step) ";
    }
	const std::string cash_str = pause_ss.str() + std::string(ICON_FA_CALENDAR) + std::string(" ") + calendar->get_date_time() + std::string(" ") + std::string(ICON_FA_LIGHTBULB_O) + power_ss.str() + std::string(ICON_FA_MONEY) + cash_ss.str();

    // Setup the main menu
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu(menu_main.c_str())) {
        if (ImGui::MenuItem(menu_main_play.c_str())) {
            game_master_mode = PLAY;
            emit_deferred(map_dirty_message{});
            emit_deferred(recalculate_mining_message{});
            world_changed=true;
        }
        if (ImGui::MenuItem(menu_main_quit.c_str())) {
            quitting = true;
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(menu_design.c_str())) {
        if (ImGui::MenuItem(menu_design_mining.c_str())) {
            game_master_mode = DESIGN;
            pause_mode = PAUSED;
            emit_deferred(map_dirty_message{});
            game_design_mode = DIGGING;
            world_changed=true;
        }
        if (ImGui::MenuItem(menu_design_building.c_str())) {
            game_master_mode = DESIGN;
            pause_mode = PAUSED;
            emit_deferred(map_dirty_message{});
            game_design_mode = BUILDING;
        }
        if (ImGui::MenuItem(menu_design_tree_cutting.c_str())) {
            game_master_mode = DESIGN;
            pause_mode = PAUSED;
            emit_deferred(map_dirty_message{});
            game_design_mode = CHOPPING;
        }
        if (ImGui::MenuItem(menu_design_guard.c_str())) {
            game_master_mode = DESIGN;
            pause_mode = PAUSED;
            emit_deferred(map_dirty_message{});
            game_design_mode = GUARDPOINTS;
        }
        if (ImGui::MenuItem(menu_design_stockpiles.c_str())) {
            game_master_mode = DESIGN;
            pause_mode = PAUSED;
            emit_deferred(map_dirty_message{});
            game_design_mode = STOCKPILES;
        }
        if (ImGui::MenuItem(menu_design_harvest.c_str())) {
            game_master_mode = DESIGN;
            pause_mode = PAUSED;
            emit_deferred(map_dirty_message{});
            game_design_mode = HARVEST;
        }
        if (ImGui::MenuItem(menu_design_architecture.c_str())) {
            game_master_mode = DESIGN;
            pause_mode = PAUSED;
            emit_deferred(map_dirty_message{});
            game_design_mode = ARCHITECTURE;
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(menu_units.c_str())) {
        if (ImGui::MenuItem(menu_units_units.c_str())) {
            game_master_mode = UNITS;
            pause_mode = PAUSED;
            emit_deferred(map_dirty_message{});
        }
        if (ImGui::MenuItem(menu_units_workflow.c_str())) {
            game_master_mode = WORKFLOW;
            pause_mode = PAUSED;
            emit_deferred(map_dirty_message{});
        }
        if (ImGui::MenuItem(menu_units_civs.c_str())) {
            game_master_mode = CIVS;
            pause_mode = PAUSED;
            emit_deferred(map_dirty_message{});
        }
        if (ImGui::MenuItem(menu_units_standing_orders.c_str())) {
            game_master_mode = STANDING_ORDERS;
            pause_mode = PAUSED;
            emit_deferred(map_dirty_message{});
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Display")) {
        if (ImGui::MenuItem("Top-Down")) {
            camera_mode = TOP_DOWN;
        }
        if (ImGui::MenuItem("Front")) {
            camera_mode = FRONT;
        }
        if (ImGui::MenuItem("Diagonal")) {
            camera_mode = DIAGONAL;
        }
        if (ImGui::MenuItem("Toggle ASCII")) {
            ascii_mode = !ascii_mode;
        }
        ImGui::EndMenu();
    }

    auto status_size = ImGui::CalcTextSize(cash_str.c_str());
    ImGui::SameLine(ImGui::GetWindowWidth()-(status_size.x + 10));
    ImGui::Text("%s", cash_str.c_str());
    ImGui::EndMainMenuBar();
}
