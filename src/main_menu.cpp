#include "main_menu.hpp"
#include <rltk.hpp>
#include "menu_helper.hpp"

constexpr int BACKDROP_LAYER=1;

using namespace rltk;
using namespace rltk::colors;

void main_menu::init() {
	gui->add_owner_layer(BACKDROP_LAYER, 0, 0, 800, 600, resize_fullscreen, draw_splash_backdrop);
}

void main_menu::destroy() {
	gui->delete_layer(BACKDROP_LAYER);
}

void main_menu::tick(const double duration_ms) {
}
