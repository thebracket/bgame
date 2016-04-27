#include <rltk.hpp>
#include "splash_screen.hpp"
#include "main_menu.hpp"
#include "world_gen.hpp"
#include "play_game.hpp"

using namespace rltk;
using namespace rltk::colors;

enum game_mode_t { SPLASH, MAIN_MENU, WORLD_GEN, PLAY_GAME };
game_mode_t mode = SPLASH;

splash_screen splash;
main_menu menu;
world_gen worldgen;
play_game game;

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
						mode = PLAY_GAME;
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
			if (game.quitting) {
				game.destroy();
				mode = MAIN_MENU;
				menu.init();
			}
		} break;
	}
}

int main()
{
	init(config_advanced("../assets", 800, 600, "Black Future"));
	splash.init();
    run(tick);
}
