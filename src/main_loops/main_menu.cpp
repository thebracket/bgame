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
#include "../bengine/IconsFontAwesome.h"
#include "../global_assets/game_config.hpp"
#include "worldgen.hpp"
#include "../bengine/telemetry.hpp"
#include "play_game.hpp"
#include <algorithm>

namespace main_menu {
    static bool initialized = false;
    static bool world_exists = false;
    static std::string tagline = "";
    static bool show_options = false;
    const std::string win_options = std::string(ICON_FA_WRENCH) + " Options";
    const std::string btn_save = std::string(ICON_FA_FLOPPY_O) + " Save Changes";
    const std::string btn_close = std::string(ICON_FA_TIMES) + " Close";
    const std::string menu_play = std::string(ICON_FA_PLAY) + " Play the Game";
    const std::string menu_gen = std::string(ICON_FA_MAP) + " Generate the World";
    const std::string menu_opts = std::string(ICON_FA_WRENCH) + " Options";
    const std::string menu_quit = std::string(ICON_FA_TIMES) + " Quit the Game";

    static std::string online_username = "";

    static std::string get_descriptive_noun() {
        using namespace string_tables;

        bengine::random_number_generator rng;
        return string_table(MENU_SUBTITLES)->random_entry(rng);
    }

	void check_world_exists() {
		if (exists(get_save_path() + std::string("/savegame.dat"))) {
			world_exists = true;
		}
	}

    static void init() {
		check_world_exists();

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

        online_username = config::game_config.online_username;

        call_home("MainMenu", "Opened");

        initialized = true;
    }

	static float calc_indent(const float window_width, const float text_width) {
		return window_width / 2.0f - text_width / 2.0f;
	}

    void tick(const double &duration_ms) {
        if (!initialized) init();

        ImVec4 red{1.0f, 0.0f, 0.0f, 1.0f};
        ImVec4 yellow{1.0f, 1.0f, 0.0f, 1.0f};
        const std::string kylah = "To Kylah of the West and Jakie Monster - The Bravest Little Warriors of Them All.";

        int screen_w, screen_h;
        glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);


        // Display the background image centered, and the logo
        bengine::display_sprite(assets::background_image->texture_id, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
        bengine::display_sprite(assets::game_logo->texture_id, 0.4f, 0.2f, 0.5f, 0.7f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);

		ImGui_ImplGlfwGL3_NewFrame();

        // Supporters list; TODO: make this read a file
        ImGui::Begin("Thanks to our supporters:", nullptr, ImVec2{600,125}, 0.5f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
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
			constexpr int BUTTON_ADD = 20;
			const auto tagline_size = ImGui::CalcTextSize(tagline.c_str());
			const auto kylah_size = ImGui::CalcTextSize(kylah.c_str());
			const float window_width = std::max(tagline_size.x, kylah_size.x);
			const float play_size = ImGui::CalcTextSize(menu_play.c_str()).x + BUTTON_ADD;
			const float gen_size = ImGui::CalcTextSize(menu_gen.c_str()).x + BUTTON_ADD;
			const float opts_size = ImGui::CalcTextSize(menu_opts.c_str()).x + BUTTON_ADD;
			const float quit_size = ImGui::CalcTextSize(menu_quit.c_str()).x + BUTTON_ADD;
			const float tagline_indent = calc_indent(window_width, tagline_size.x);
			const float play_indent = calc_indent(window_width, play_size);
			const float gen_indent = calc_indent(window_width, gen_size);
			const float opts_indent = calc_indent(window_width, opts_size);
			const float quit_indent = calc_indent(window_width, quit_size);
			const float kylah_indent = calc_indent(window_width, kylah_size.x);

			ImGui::SetNextWindowPosCenter();
            ImGui::Begin("MainMenu", nullptr, ImVec2{window_width, 400}, 0.0f,
                         ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoTitleBar);
			ImGui::SetCursorPosX(tagline_indent);
            ImGui::TextColored(red, "%s", tagline.c_str());
            if (world_exists) {
				ImGui::SetCursorPosX(play_indent);
                if (ImGui::Button(menu_play.c_str())) {
                    bengine::main_func = play_game::tick;
                }
            }
			ImGui::SetCursorPosX(gen_indent);
			if (ImGui::Button(menu_gen.c_str())) {
                bengine::main_func = worldgen::tick;
            }
			ImGui::SetCursorPosX(opts_indent);
            if (ImGui::Button(menu_opts.c_str())) {
                show_options = true;
				call_home("game", "Options");
            }
			ImGui::SetCursorPosX(quit_indent);
			if (ImGui::Button(menu_quit.c_str())) {
                //stop_telemetry();
				call_home("game", "Quit");
                glfwSetWindowShouldClose(bengine::main_window, true);
            }
			ImGui::SetCursorPosX(kylah_indent);
            ImGui::TextColored(red, "%s", kylah.c_str());
            ImGui::End();
        } else {
            using namespace config;

            // Show the options window
            ImGui::SetNextWindowPosCenter();
            ImGui::Begin(win_options.c_str(), nullptr, ImVec2{600, 400}, 0.7f,
                         ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
            ImGui::Text("Options won't take effect until you restart.");
            ImGui::Text("Full Screen Mode");
            ImGui::SameLine();
            ImGui::Checkbox("## Full Screen", &game_config.fullscreen);
            ImGui::Text("Window Height");
            ImGui::SameLine();
            ImGui::InputInt("## Window Mode Width", &game_config.window_width);
            ImGui::SameLine();
            ImGui::Text("Width");
            ImGui::SameLine();
            ImGui::InputInt(" ## Window Mode Height", &game_config.window_height);
            ImGui::Text("Autosave Every X Minutes (0=never)");
            ImGui::SameLine();
            ImGui::InputInt("## Auto save every X minutes (0 none)", &game_config.autosave_minutes);
            ImGui::Text("Allow telemetry to phone home");
            ImGui::SameLine();
            ImGui::Checkbox("## Allow telemetry to phone home", &game_config.allow_calling_home);
            ImGui::Text("Online Username");
            ImGui::SameLine();
            ImGui::InputText("Online Username", (char *) &online_username, 254);
            ImGui::Text("UI Scale Factor");
            ImGui::InputFloat("## Scale Factor", &game_config.scale_factor);
            ImGui::Text("Show Entity ID Numbers");
            ImGui::SameLine();
            ImGui::Checkbox("## Entity ID", &game_config.show_entity_ids);
            if (ImGui::Button(btn_save.c_str())) {
                game_config.online_username = std::string(online_username);
                game_config.save();
                show_options = false;
            }
            ImGui::SameLine();
            if (ImGui::Button(btn_close.c_str())) {
                show_options = false;
            }
            ImGui::End();
        }

        ImGui::Render();
    }
}
