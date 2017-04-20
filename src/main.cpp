#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>
#include <rltk.hpp>
#include "main/splash_screen.hpp"
#include "main/main_menu.hpp"
#include "main/world_gen.hpp"
#include "main/play_game.hpp"
#include "main/game_config.hpp"
#include "main/game_globals.hpp"
#include "main/game_planet.hpp"
#include "main/guitheme.hpp"
#include "utils/string_utils.hpp"
#include "main/IconsFontAwesome.h"
#include "external/imgui-sfml/imgui-SFML.h"
#include "utils/telemetry.hpp"
#include "utils/string_utils.hpp"
#include <chrono>
#include <filesystem.hpp>
#include <unistd.h>
#include "utils/filesystem.hpp"
#ifdef __APPLE__
#include <libproc.h>
#endif
#include <GL/glew.h>

using namespace rltk;
using namespace rltk::colors;

enum game_mode_t { SPLASH, MAIN_MENU, WORLD_GEN, PLAY_GAME };
game_mode_t mode = SPLASH;

splash_screen splash;
main_menu menu;
world_gen worldgen;
play_game game;
std::chrono::high_resolution_clock::time_point last_save;

void save_game() {
	const std::string save_filename = get_save_path() + std::string("/savegame.dat");
	std::cout << "Saving game to " << save_filename << "\n";
	if (exists(save_filename)) std::remove(save_filename.c_str());
	std::unique_ptr<std::ofstream> lbfile = std::make_unique<std::ofstream>(save_filename, std::ios::out | std::ios::binary);
	ecs_save(lbfile);
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
        io.Fonts->AddFontFromFileTTF(font_path.c_str(), game_config.gui_ttf_size);
        ImFontConfig config;
        config.MergeMode = true;
        const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        io.Fonts->AddFontFromFileTTF("assets/fontawesome-webfont.ttf", game_config.gui_ttf_size, &config, icon_ranges);

        // Initialize
        ImGui::SFML::Init(*rltk::get_window());
        ImGui::SetupImGuiStyle();
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
		if (split_line[0] == "scale_factor") game_config.scale_factor = std::stof(split_line[1]);
		if (split_line[0] == "allow_calling_home" && split_line[1] != "yes") game_config.allow_calling_home = false;
		if (split_line[0] == "online_username") game_config.online_username = split_line[1];
        if (split_line[0] == "show_entity_ids" && split_line[1]!="yes") game_config.show_entity_ids = false;
	}
	rltk::scale_factor = game_config.scale_factor;
}

void remove_from_path(std::string &s, const std::string needle) {
    //std::cout << "Searching " << s << " for " << needle << "\n";
    const std::size_t location = s.find_last_of(needle);
    if (location == std::string::npos) {
        //std::cout << "Not found\n";
    } else {
        //std::cout << "Found at location " << location << "\n";
        s = s.substr(0, location);
    }
}

int main(int argc, char* argv[])
{

#ifdef __APPLE__
    // We're probably running inside a bundle, which means that we need to find the executable's path
    // and have that as our working directory

    int ret;
    pid_t pid;
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];

    std::string executable_path;

    pid = getpid();
    ret = proc_pidpath (pid, pathbuf, sizeof(pathbuf));
    if ( ret <= 0 ) {
        fprintf(stderr, "PID %d: proc_pidpath ();\n", pid);
        fprintf(stderr, "    %s\n", strerror(errno));
    } else {
        //printf("proc %d: %s\n", pid, pathbuf);
        executable_path = pathbuf;
    }

    remove_from_path(executable_path, std::string("/"));

    std::cout << executable_path << "\n";
    chdir(executable_path.c_str());

#endif // apple


	read_config();
    start_telemetry();
	init(config_advanced("assets", game_config.window_width, game_config.window_height, "Black Future",game_config.fullscreen));
	glewExperimental = GL_TRUE;
	glewInit();
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
    call_home("Startup");
    call_home("Startup-FullScreen", std::to_string(game_config.fullscreen));
    call_home("Startup-Resolution", std::to_string(game_config.window_width) + std::string("x") + std::to_string(game_config.window_height));
    call_home("Startup-GUI-Font", game_config.gui_ttf + std::string(" / ") + std::to_string(game_config.gui_ttf_size));
    call_home("Startup-Scaling", std::to_string(game_config.scale_factor));
    run(tick);
    call_home("Shutdown");
    stop_telemetry();
}
