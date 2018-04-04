#include "stdafx.h"
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
	static const std::string win_options = std::string(ICON_FA_WRENCH) + " Options";
	static const std::string btn_save = std::string(ICON_FA_FLOPPY_O) + " Save Changes";
	static const std::string btn_close = std::string(ICON_FA_TIMES) + " Close";
	static const std::string menu_play = std::string(ICON_FA_PLAY) + " Play the Game";
	static const std::string menu_gen = std::string(ICON_FA_MAP) + " Generate the World";
	static const std::string menu_opts = std::string(ICON_FA_WRENCH) + " Options";
	static const std::string menu_quit = std::string(ICON_FA_TIMES) + " Quit the Game";

	static const char * texture_size_items = "Tiny (128x128)\0Small (256x256)\0Medium (512x512)\0Large (1024x1024)\0Huge (2048x2048)\0Enormous (4096x4096)\0Maximum (8096x8096)\0\0";
	static const char * shadowmap_size_items = "Tiny (32x32)\0Small (64x64)\0Medium (128x128)\0Large (256x256)\0Huge (512x512)\0Maximum (1024x1024)\0\0";

    static std::string online_username;

	static int selected_texture_size = 0;
	static int selected_shadowmap_size = 0;

    static std::string get_descriptive_noun() {
        using namespace string_tables;

        const bengine::random_number_generator rng;
        return string_table(MENU_SUBTITLES)->random_entry(rng);
    }

	void check_world_exists() noexcept {
		if (exists(save_filename())) {
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

        const auto first_noun = get_descriptive_noun();
        auto second_noun = first_noun;
        while (second_noun == first_noun) {
            second_noun = get_descriptive_noun();
        }

        tagline += "of " + first_noun + " and " + second_noun;

        online_username = config::game_config.online_username;

		switch (config::game_config.texture_size)
		{
		case 128: selected_texture_size = 0; break;
		case 256: selected_texture_size = 1; break;
		case 512: selected_texture_size = 2; break;
		case 1024: selected_texture_size = 3; break;
		case 2048: selected_texture_size = 4; break;
		case 4096: selected_texture_size = 5; break;
		case 8096: selected_texture_size = 6; break;
		default: selected_texture_size = 0;
		}

		switch (config::game_config.shadow_map_size)
		{
		case 32: selected_shadowmap_size = 0; break;
		case 64: selected_shadowmap_size = 1; break;
		case 128: selected_shadowmap_size = 2; break;
		case 256: selected_shadowmap_size = 3; break;
		case 512: selected_shadowmap_size = 4; break;
		case 1024: selected_shadowmap_size = 5; break;
		}

        call_home("MainMenu", "Opened");

        initialized = true;
    }

	static float calc_indent(const float window_width, const float text_width) {
		return window_width / 2.0f - text_width / 2.0f;
	}

    void tick(const double &duration_ms) noexcept {
		if (!initialized) {
			init();
		}

        const ImVec4 red{1.0f, 0.0f, 0.0f, 1.0f};
        const ImVec4 yellow{1.0f, 1.0f, 0.0f, 1.0f};
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
			constexpr auto button_add = 20;
			const auto tagline_size = ImGui::CalcTextSize(tagline.c_str());
			auto kylah_size = ImGui::CalcTextSize(kylah.c_str());
			kylah_size.x += 50;
			const auto window_width = std::max(tagline_size.x, kylah_size.x);
			const auto play_size = ImGui::CalcTextSize(menu_play.c_str()).x + button_add;
			const auto gen_size = ImGui::CalcTextSize(menu_gen.c_str()).x + button_add;
			const auto opts_size = ImGui::CalcTextSize(menu_opts.c_str()).x + button_add;
			const auto quit_size = ImGui::CalcTextSize(menu_quit.c_str()).x + button_add;
			const auto tagline_indent = calc_indent(window_width, tagline_size.x);
			const auto play_indent = calc_indent(window_width, play_size);
			const auto gen_indent = calc_indent(window_width, gen_size);
			const auto opts_indent = calc_indent(window_width, opts_size);
			const auto quit_indent = calc_indent(window_width, quit_size);
			const auto kylah_indent = calc_indent(window_width + 12.0f, kylah_size.x);

			ImGui::SetNextWindowPosCenter();
            ImGui::Begin("MainMenu", nullptr, ImVec2{window_width + 25.0f, 400}, 0.0f,
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

			ImGui::Text("Texture Size");
			ImGui::SameLine();
			ImGui::Combo("## TexSize", &selected_texture_size, texture_size_items);

			ImGui::Text("Shadow Map Size");
			ImGui::SameLine();
			ImGui::Combo("## ShadowSize", &selected_shadowmap_size, shadowmap_size_items);

			ImGui::Text("Always update shadows (slower)");
			ImGui::SameLine();
			ImGui::Checkbox("## Always shadow", &game_config.always_update_shadows);

			ImGui::Text("Mip Levels (0 = automatic, square root of texture size)");
			ImGui::SameLine();
			ImGui::InputInt("##MIP", &game_config.mip_levels, 1, 1);

			ImGui::Text("Render ASCII lighting");
			ImGui::SameLine();
			ImGui::Checkbox("## ASCII lighting", &game_config.render_ascii_light);

			ImGui::Text("Disable 3D lighting");
			ImGui::SameLine();
			ImGui::Checkbox("## 3D lighting", &game_config.disable_lighting);

			ImGui::Text("Disable HDR");
			ImGui::SameLine();
			ImGui::Checkbox("## HDR", &game_config.disable_hdr);

			ImGui::Text("Disable Screen Space Ambient Occlusion");
			ImGui::SameLine();
			ImGui::Checkbox("## SSAO", &game_config.disable_ssao);

			ImGui::Text("ASCII mode - number of levels down to look (0 disables)");
			ImGui::SameLine();
			ImGui::InputInt("##asciidive", &game_config.num_ascii_levels_below, 1, 1);

			ImGui::Text("Shadow divisor; higher is less-frequent shadow updates");
			ImGui::SameLine();
			ImGui::InputInt("##shadowdiv", &game_config.shadow_divisor, 1, 1);
			if (game_config.shadow_divisor < 1) game_config.shadow_divisor = 1;

			ImGui::Text("Ticks per ms. Recommended at 33.");
			ImGui::SameLine();
			ImGui::InputFloat("##ticksperms", &game_config.ticks_per_ms, 1, 1);

			ImGui::Text("Vsync");
			ImGui::SameLine();
			ImGui::Checkbox("## vsync", &game_config.vsync);

			ImGui::Text("Parallax");
			ImGui::SameLine();
			ImGui::Checkbox("## parallax", &game_config.parallax);

            if (ImGui::Button(btn_save.c_str())) {
				switch (selected_texture_size)
				{
				case 0: game_config.texture_size = 128; break;
				case 1: game_config.texture_size = 256; break;
				case 2: game_config.texture_size = 512; break;
				case 3: game_config.texture_size = 1024; break;
				case 4: game_config.texture_size = 2048; break;
				case 5: game_config.texture_size = 4096; break;
				case 6: game_config.texture_size = 8192; break;
				default: game_config.texture_size = 512;
				}

				switch (selected_shadowmap_size)
				{
				case 0: game_config.shadow_map_size = 32; break;
				case 1: game_config.shadow_map_size = 64; break;
				case 2: game_config.shadow_map_size = 128; break;
				case 3: game_config.shadow_map_size = 256; break;
				case 4: game_config.shadow_map_size = 512; break;
				case 5: game_config.shadow_map_size = 1024; break;
				}

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
