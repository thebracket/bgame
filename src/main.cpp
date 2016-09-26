#include <rltk.hpp>
#include "main/splash_screen.hpp"
#include "main/main_menu.hpp"
#include "main/world_gen.hpp"
#include "main/play_game.hpp"
#include "main/game_globals.hpp"
#include "utils/string_utils.hpp"
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
	std::fstream lbfile(save_filename, std::ios::out | std::ios::binary);
	ecs_save(lbfile);
	std::cout << "Saving region\n";
	save_region(*current_region);
	last_save = std::chrono::high_resolution_clock::now();
}

void tick(double duration_ms) {
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
				mode = MAIN_MENU;
				menu.init();
			} else if (game_config.autosave_minutes > 0) {
				double duration_minutes = static_cast<double>(std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - last_save).count());
				if (duration_minutes > game_config.autosave_minutes) save_game();
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
		if (split_line[0] == "gui_font") game_config.gui_font = split_line[1];
		if (split_line[0] == "tooltip_fadein" && split_line[1] != "yes") game_config.tooltip_fadein = false;
		if (split_line[0] == "tooltip_speed") game_config.tooltip_speed = std::stof(split_line[1]);
		if (split_line[0] == "autosave_minutes") game_config.autosave_minutes = std::stoi(split_line[1]);
	}
}

int main(int argc, char* argv[])
{
#ifndef WIN32

	fs::path full_path( fs::initial_path<fs::path>() );
	full_path = fs::system_complete( fs::path( argv[0] ) );
	std::cout << full_path << std::endl;
	std::cout << full_path.parent_path() << std::endl;
	fs::current_path(full_path.parent_path());

#endif // !WIN32

	read_config();
	init(config_advanced("assets", game_config.window_width, game_config.window_height, "Black Future"));
	splash.init();
    run(tick);
}
