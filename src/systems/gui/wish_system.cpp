#include "wish_system.hpp"
#include "../../messages/messages.hpp"
#include "../ai/distance_map_system.hpp"
#include <../../external/imgui-sfml/imgui-SFML.h>
#include "../physics/fluid_system.hpp"
#include "mode_play_system.hpp"
#include "../../planet/region/region.hpp"
#include "../../main/game_mode.hpp"
#include "../input/mouse_input_system.hpp"
#include "../../components/explosion_t.hpp"
#include "../../components/position.hpp"
#include "../../main/game_rng.hpp"

using namespace rltk;
using namespace region;

namespace boomtest {
    bool boom_mode = false;
    int boom_timer = 20;
    int boom_radius = 5;
    int boom_damage_n = 1;
    int boom_damage_d = 20;
}

void wish_system::configure() {
    system_name = "Make A Wish";
}

void wish_system::make_wish(const std::string &wish) {
    if (wish == "sploosh") {
        std::cout << "Let it rain!\n";
        for (int y=1; y<REGION_HEIGHT-1; ++y) {
            for (int x=1; x<REGION_WIDTH-1; ++x) {
                set_water_level(mapidx(x,y,REGION_DEPTH-2), 3);
                water_stable[mapidx(x,y,REGION_DEPTH-2)]=false;
            }
        }
    } else if (wish == "show distance") {
        dijkstra_debug = !dijkstra_debug;
    } else if (wish == "show flags") {
        flags_debug = !flags_debug;
    } else if (wish == "boom") {
        boomtest::boom_mode = true;
    } else if (wish == "asteroids") {
        const int n_missiles = rng.roll_dice(3,10);
        for (int i=0; i<n_missiles; ++i) {
            const int x = rng.roll_dice(1, REGION_WIDTH-1);
            const int y = rng.roll_dice(1, REGION_HEIGHT-1);
            const int z = ground_z(x, y);
            create_entity()
                    ->assign(position_t{ x, y, z })
                    ->assign(explosion_t{10, 10, 5, 20});
        }
    } else if (wish == "nuke") {
        const int x = REGION_WIDTH/2 - 20;
        const int y = REGION_HEIGHT/2 - 20;
        const int z = std::max(REGION_DEPTH-2, ground_z(x, y));
        create_entity()
                ->assign(position_t{ x, y, z })
                ->assign(explosion_t{10, 30, 10, 200});
    }

    if (!boomtest::boom_mode) {
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
}

void wish_system::update(const double ms) {
    if (game_master_mode != WISHMODE) return;

    if (!boomtest::boom_mode) {
        ImGui::Begin("Make A Wish - Debug Command", nullptr, ImVec2{400, 100}, ImGuiWindowFlags_AlwaysAutoResize);
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
    } else {
        ImGui::Begin("Explosion parameters");
        ImGui::InputInt("Timer", &boomtest::boom_timer);
        ImGui::InputInt("Radius", &boomtest::boom_radius);
        ImGui::InputInt("N Dice", &boomtest::boom_damage_n);
        ImGui::InputInt("Dice Type", &boomtest::boom_damage_d);
        ImGui::End();

        if (mouse::clicked && mouse::mouse_world_x > 0 && mouse::mouse_world_x < REGION_WIDTH
                && mouse::mouse_world_y > 0 && mouse::mouse_world_y < REGION_HEIGHT
                && mouse::mouse_world_z > 0 && mouse::mouse_world_z < REGION_DEPTH) {
            boomtest::boom_mode = false;
            create_entity()
                    ->assign(position_t{mouse::mouse_world_x, mouse::mouse_world_y, mouse::mouse_world_z})
                    ->assign(explosion_t{ (uint8_t)boomtest::boom_timer, (uint16_t)boomtest::boom_radius, (uint8_t)boomtest::boom_damage_n, (uint16_t)boomtest::boom_damage_d });
            game_master_mode = PLAY;
            emit_deferred(map_dirty_message{});
            emit_deferred(recalculate_mining_message{});
        }
    }
}