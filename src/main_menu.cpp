#include "main_menu.hpp"
#include <rltk.hpp>
#include "menu_helper.hpp"
#include "constants.hpp"

constexpr int BACKDROP_LAYER=1;
constexpr int LOG_LAYER=2;

using namespace rltk;
using namespace rltk::colors;

void main_menu::init() {
	gui->add_owner_layer(BACKDROP_LAYER, 0, 0, 800, 600, resize_fullscreen, draw_splash_backdrop);
	gui->add_layer(LOG_LAYER, 0, 0, 800, 600, "8x16", resize_fullscreen, false);
}

void main_menu::destroy() {
	gui->delete_layer(BACKDROP_LAYER);
	gui->delete_layer(LOG_LAYER);
}

void main_menu::tick(const double duration_ms) {
	const int y_height = term(LOG_LAYER)->term_height;
	const int y_center = y_height / 2;

	term(LOG_LAYER)->clear();
	term(LOG_LAYER)->print_center(y_center - 6, VERSION, WHITE, BLACK);
	term(LOG_LAYER)->print_center(y_center - 5, "Powered by RLTK - the RogueLike Tool Kit", RED, BLACK);

	if (selected != 0) {
		term(LOG_LAYER)->print_center(y_center - 3, "Play the Game", GREY, BLACK);
	} else {
		term(LOG_LAYER)->print_center(y_center - 3, "Play the Game", YELLOW, BLACK);
	}

	if (selected != 1) {
		term(LOG_LAYER)->print_center(y_center - 2, "Generate The World", GREY, BLACK);
	} else {
		term(LOG_LAYER)->print_center(y_center - 2, "Generate The World", YELLOW, BLACK);
	}

	if (selected != 2) {
		term(LOG_LAYER)->print_center(y_center - 1, "Quit", GREY, BLACK);
	} else {
		term(LOG_LAYER)->print_center(y_center - 1, "Quit", YELLOW, BLACK);
	}
}
