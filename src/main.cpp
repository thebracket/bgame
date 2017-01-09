#include <rltk.hpp>
#include "main/splash_screen.hpp"
#include "main/main_menu.hpp"
#include "main/world_gen.hpp"
#include "main/play_game.hpp"
#include "main/game_globals.hpp"
#include "main/guitheme.hpp"
#include "utils/string_utils.hpp"
#include "external/imgui-sfml/imgui-SFML.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <chrono>

using namespace rltk;
using namespace rltk::colors;
namespace fs = boost::filesystem;

enum game_mode_t { SPLASH, MAIN_MENU, WORLD_GEN, PLAY_GAME };
game_mode_t mode = SPLASH;

splash_screen splash;
main_menu menu;
world_gen worldgen;
play_game game;
std::chrono::high_resolution_clock::time_point last_save;

void save_game() {
	const std::string save_filename = "world/savegame.dat";
	std::cout << "Saving game to " << save_filename << "\n";
	if (boost::filesystem::exists(save_filename)) boost::filesystem::remove(save_filename);
	std::unique_ptr<std::ofstream> lbfile = std::make_unique<std::ofstream>(save_filename, std::ios::out | std::ios::binary);
	ecs_save(std::move(lbfile));
	std::cout << "Saving region\n";
	save_region(*current_region);
	std::cout << "Saving planet\n";
	save_planet(planet);
	last_save = std::chrono::high_resolution_clock::now();
}

bool has_init = false;
sf::Clock deltaClock;

void tick(double duration_ms) {
    // Initial ImGui call
    if (!has_init) {
        ImGuiIO& io = ImGui::GetIO();
		const std::string font_path = std::string("assets/") + game_config.gui_ttf;
        //io.Fonts->AddFontDefault();
        std::cout << "Loading " << font_path << ", at size " << game_config.gui_ttf_size << " pixels\n";
        ImFont * my_font = io.Fonts->AddFontFromFileTTF(font_path.c_str(), game_config.gui_ttf_size);
        ImGui::SFML::Init(*rltk::get_window());
        ImGui::SetupImGuiStyle(true, 0.8f);
        has_init = true;
    }
    ImGui::SFML::Update(*rltk::get_window(), deltaClock.restart());

	switch (mode) {
		case SPLASH : {
			splash.tick(duration_ms);
			if (splash.done_loading) {
				splash.destroy();
				mode = MAIN_MENU;
				menu.init();
			} 
		} break;
		case MAIN_MENU : {
			menu.tick(duration_ms);
			if (menu.clicked) {
				switch (menu.selected) {
					case 0 : { 
						/* TODO: Play Game */ 
						menu.destroy();
						quitting = false;
						mode = PLAY_GAME;
						last_save = std::chrono::high_resolution_clock::now();
						game.init();
					} break;
					case 1 : {
						// World gen
						menu.destroy();
						worldgen.init();
						mode = WORLD_GEN;
					} break;
					
					case 2 : { 
						// Quit
						get_window()->close(); 
					} break;
				}
			}
		} break;
		case WORLD_GEN : {
			worldgen.tick(duration_ms);
			if (worldgen.done) {
				worldgen.destroy();
				mode = MAIN_MENU;
				menu.init();
			}
		} break;
		case PLAY_GAME : {
			game.tick(duration_ms);
			ecs_garbage_collect();
			if (quitting) {
				save_game();
				game.destroy();
				delete_all_entities();
				delete_all_systems();
				mode = MAIN_MENU;
				menu.init();
			} else if (game_config.autosave_minutes > 0) {
				double duration_minutes = static_cast<double>(std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - last_save).count());
				if (duration_minutes >= game_config.autosave_minutes) {
					term(1)->clear();
					term(2)->clear();
					term(3)->clear();
					term(1)->print_center(10, "Auto-saving game - please wait");
					gui->render(*get_window());
					get_window()->display();
					save_game();
				}
			}
		} break;
	}
}

void read_config() {
	std::ifstream f("world_defs/config.txt");
	std::string line;
	while (getline(f, line))
	{
		auto split_line = split(line, '=');
		if (split_line[0] == "window_width") game_config.window_width = std::stoi(split_line[1]);
		if (split_line[0] == "window_height") game_config.window_height = std::stoi(split_line[1]);
		if (split_line[0] == "game_font") game_config.game_font = split_line[1];
		if (split_line[0] == "game_font_small") game_config.game_font_small = split_line[1];
		if (split_line[0] == "gui_font") game_config.gui_font = split_line[1];
		if (split_line[0] == "tooltip_fadein" && split_line[1] != "yes") game_config.tooltip_fadein = false;
		if (split_line[0] == "tooltip_speed") game_config.tooltip_speed = std::stof(split_line[1]);
		if (split_line[0] == "autosave_minutes") game_config.autosave_minutes = std::stoi(split_line[1]);
		if (split_line[0] == "fullscreen" && split_line[1]=="1") game_config.fullscreen = true;
		if (split_line[0] == "gui_ttf") game_config.gui_ttf = split_line[1];
		if (split_line[0] == "gui_ttf_size") game_config.gui_ttf_size = std::stoi(split_line[1]);

	}
}

int main(int argc, char* argv[])
{
    /*
#ifndef WIN32

	fs::path full_path( fs::initial_path<fs::path>() );
	full_path = fs::system_complete( fs::path( argv[0] ) );
	fs::current_path(full_path.parent_path());

#endif // !WIN32
     */

	read_config();
	init(config_advanced("assets", game_config.window_width, game_config.window_height, "Black Future",game_config.fullscreen));
	splash.init();

    // ImGui hooks
    std::function<bool(sf::Event)> on_message = [] (sf::Event e) {
        ImGui::SFML::ProcessEvent(e);
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureKeyboard || io.WantCaptureMouse) {
            return false;
        }
        return true;
    };
    rltk::optional_event_hook = on_message;

    std::function<void()> on_display = [] () {
        ImGui::Render();
    };
    rltk::optional_display_hook = on_display;

    // Main loop - hand over to RLTK
    run(tick);
}
