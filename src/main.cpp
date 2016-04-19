#include <rltk.hpp>
#include "splash_screen.hpp"
#include "main_menu.hpp"

using namespace rltk;
using namespace rltk::colors;

enum game_mode_t { SPLASH, MAIN_MENU };
game_mode_t mode = SPLASH;

splash_screen splash;
main_menu menu;

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
		} break;
	}
}

int main()
{
	init(config_advanced("../assets", 800, 600, "Black Future"));
	splash.init();
    run(tick);
}
