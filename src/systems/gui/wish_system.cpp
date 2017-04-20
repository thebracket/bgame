#include "wish_system.hpp"
#include "../input/keyboard_system.hpp"
#include "../../messages/messages.hpp"
#include "../ai/distance_map_system.hpp"
#include <../../external/imgui-sfml/imgui-SFML.h>
#include "../physics/fluid_system.hpp"
#include "mode_play_system.hpp"
#include "../../main/game_region.hpp"
#include "../../main/game_mode.hpp"

using namespace rltk;

void wish_system::configure() {
    system_name = "Make A Wish";
}

void wish_system::make_wish(const std::string &wish) {
    if (wish == "sploosh") {
        std::cout << "Let it rain!\n";
        for (int y=1; y<REGION_HEIGHT-1; ++y) {
            for (int x=1; x<REGION_WIDTH-1; ++x) {
                current_region->water_level[mapidx(x,y,REGION_DEPTH-2)] = 3;
                water_stable[mapidx(x,y,REGION_DEPTH-2)]=false;
            }
        }
    } else if (wish == "show distance") {
        dijkstra_debug = !dijkstra_debug;
    } else if (wish == "show flags") {
        flags_debug = !flags_debug;
    } else if (wish == "reveal all") {
        std::fill(current_region->revealed.begin(), current_region->revealed.end(), true);
        std::fill(current_region->visible.begin(), current_region->visible.end(), true);
        emit_deferred(map_dirty_message{});
    }

    game_master_mode = PLAY;
    emit_deferred(map_dirty_message{});
    emit_deferred(recalculate_mining_message{});
}

void wish_system::update(const double ms) {
    if (game_master_mode != WISHMODE) return;

    ImGui::Begin("Make A Wish - Debug Command", nullptr, ImVec2{400,100}, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::InputText("Wish Command", wish_command, 255);
    if (ImGui::Button("Wish!")) {
        std::string wishstring(wish_command);
        make_wish(wishstring);
    }
    ImGui::SameLine();
    if (ImGui::Button("Close")) {
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
    ImGui::End();
}