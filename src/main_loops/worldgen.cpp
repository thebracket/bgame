#include "worldgen.hpp"
#include "../bengine/IconsFontAwesome.h"
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"
#include "../bengine/telemetry.hpp"
#include "../global_assets/texture_storage.hpp"
#include "../bengine/simple_sprite.hpp"
#include "../bengine/textures.hpp"
#include "../bengine/main_window.hpp"
#include "main_menu.hpp"
#include "../planet/planet_builder.hpp"

namespace worldgen {
    enum world_gen_mode_t { WG_MENU, WG_RUNNING };
    bool initialized = false;
    int seed;
    int plains = 3;
    int water = 3;
    int starting_settlers = 10;
    bool strict_beamdown = false;
    world_gen_mode_t mode = WG_MENU;
    std::unique_ptr<std::thread> world_thread;

    void init() {
        call_home("WorldGen", "Opened");
        initialized = true;
    }

    void start_thread() {
        world_thread = std::make_unique<std::thread>(build_planet, seed, water, plains, starting_settlers, strict_beamdown);
    }

    void render_menu() {
        bengine::display_sprite(assets::background_image->texture_id);
        ImGui::Begin("World Generation", nullptr, ImVec2{600,125}, 0.5f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);

        ImGui::InputInt("World Seed", &seed);
        ImGui::SliderInt("Water Level", &water, 1, 4);
        ImGui::SliderInt("Plains Level", &plains, 1, 4);
        ImGui::SliderInt("Starting Settlers", &starting_settlers, 1, 20);
        ImGui::Checkbox("Require Teleport Beacon for Beamdown", &strict_beamdown);
        if (ImGui::Button("Create World")) {
            mode = WG_RUNNING;
            start_thread();
        }
        ImGui::SameLine();
        if (ImGui::Button("Back to main menu")) {
            bengine::main_func = main_menu::tick;
        }
        ImGui::End();
    }

    void tick(const double &duration_ms) {
        if (!initialized) init();

        ImGui_ImplGlfwGL3_NewFrame();

        if (mode == WG_MENU) {
            render_menu();
        } else {
            // Render status
            planet_builder_lock.lock();
            ImGui::Begin("World Generation Progress");
            ImGui::Text("%s",planet_builder_status.c_str());
            ImGui::End();
            planet_builder_lock.unlock();

            // TODO: Render progress

            if (is_planet_build_complete()) {
                bengine::main_func = main_menu::tick;
            }
        }

        ImGui::Render();
    }
}