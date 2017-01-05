#include "wish_system.hpp"
#include "../../main/game_globals.hpp"
#include "../input/keyboard_system.hpp"
#include "../../messages/messages.hpp"
#include <../../external/imgui-sfml/imgui-SFML.h>

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
            }
        }
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
    if (ImGui::Button("Close")) {
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
    ImGui::End();
}