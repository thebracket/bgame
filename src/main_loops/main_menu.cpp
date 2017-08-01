#include <iostream>
#include "main_menu.hpp"
#include "../bengine/simple_sprite.hpp"
#include "../global_assets/texture_storage.hpp"
#include "../bengine/textures.hpp"
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"
#include "../bengine/random_number_generator.hpp"
#include "../raws/string_table.hpp"
#include "../bengine/filesystem.hpp"
#include "../bengine/main_window.hpp"
#include "../bengine/gl_include.hpp"

namespace main_menu {
    bool initialized = false;
    bool world_exists = false;
    std::string tagline = "";
    bool show_options = false;

    std::string get_descriptive_noun() {
        using namespace string_tables;

        bengine::random_number_generator rng;
        return string_table(MENU_SUBTITLES)->random_entry(rng);
    }

    void init() {
        if (exists(get_save_path() + std::string("/savegame.dat"))) {
            world_exists = true;
        }

        bengine::random_number_generator rng;
        switch (rng.roll_dice(1,8)) {
            case 1 : tagline = "Histories "; break;
            case 2 : tagline = "Chronicles "; break;
            case 3 : tagline = "Sagas "; break;
            case 4 : tagline = "Annals "; break;
            case 5 : tagline = "Narratives "; break;
            case 6 : tagline = "Recitals "; break;
            case 7 : tagline = "Tales "; break;
            case 8 : tagline = "Stories "; break;
        }

        auto first_noun = get_descriptive_noun();
        std::string second_noun = first_noun;
        while (second_noun == first_noun) {
            second_noun = get_descriptive_noun();
        }

        tagline += "of " + first_noun + " and " + second_noun;

        initialized = true;
    }

    void tick(const double &duration_ms) {
        if (!initialized) init();

        ImVec4 red{1.0f, 0.0f, 0.0f, 1.0f};
        ImVec4 yellow{1.0f, 1.0f, 0.0f, 1.0f};
        const std::string kylah = "To Kylah of the West and Jakie Monster - The Bravest Little Warriors of Them All.";

        int screen_w, screen_h;
        glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

        ImGui_ImplGlfwGL3_NewFrame();

        // Display the background image centered, and the logo
        bengine::display_sprite(assets::background_image->texture_id);
        bengine::display_sprite(assets::game_logo->texture_id, 0.4f, 0.2f, 0.5f, 0.7f);

        // Supporters list; TODO: make this read a file
        ImGui::Begin("Thanks to our awesome supporters:", nullptr, ImVec2{600,125}, 0.5f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
        ImGui::BulletText("%s", "Noah Bogart via Patreon");
        ImGui::End();

        // Taglines

        ImGui::SetNextWindowPos(ImVec2{100, static_cast<float>(screen_h - 50)});
        ImGui::Begin("Copyright", nullptr, ImVec2{400, 400}, 0.0f,
                     ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoTitleBar);
        ImGui::TextColored(yellow, "%s", "(c) 2015-2017 Bracket Productions, All Rights Reserved");
        ImGui::End();

        // Main Menu Buttons
        if (!show_options) {
            ImGui::SetNextWindowPosCenter();
            ImGui::Begin("MainMenu", nullptr, ImVec2{400, 400}, 0.0f,
                         ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoTitleBar);
            ImGui::TextColored(red, "%s", tagline.c_str());
            if (world_exists) {
                if (ImGui::Button("Play the Game")) {
                }
            }
            if (ImGui::Button("Generate the World")) {
            }
            if (ImGui::Button("Options")) {
            }
            if (ImGui::Button("Quit")) {
                glfwSetWindowShouldClose(bengine::main_window, true);
            }
            ImGui::TextColored(red, "%s", kylah.c_str());
            ImGui::End();
        } else {
            // Show the options window
        }

        ImGui::Render();
    }
}
