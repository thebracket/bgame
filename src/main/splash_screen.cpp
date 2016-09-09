#include <rltk.hpp>
#include "splash_screen.hpp"
#include "menu_helper.hpp"
#include "constants.hpp"
#include "../raws/raws.hpp"
#include "game_globals.hpp"
#include <iostream>
#include <atomic>
#include <thread>
#include <memory>

constexpr int LOG_LAYER=1;
constexpr int BACKDROP_LAYER=2;

using namespace rltk;
using namespace rltk::colors;

std::atomic<bool> splash_loader_started;
std::atomic<bool> splash_loader_complete;
std::unique_ptr<std::thread> splash_loader_thread;

void splash_loader() {
	load_raws();
	splash_loader_complete.store(true);
}

void splash_screen::tick(const double duration_ms) {
	term(LOG_LAYER)->clear();
	term(LOG_LAYER)->print_center(2, VERSION, WHITE, BLACK);
	term(LOG_LAYER)->print_center(4, "Loading assets - please wait.", YELLOW, BLACK);

	if (splash_loader_started.load() == false) {
		splash_loader_started.store(true);
		splash_loader_thread = std::make_unique<std::thread>(splash_loader);
	}

	if (splash_loader_complete.load()) {
		splash_loader_thread->join();
		splash_loader_thread.reset();
		done_loading = true;
	}
}

void splash_screen::init() {
	register_texture("assets/background_image.png", "backdrop");
	register_texture("assets/gamelogo.png", "logo");

	auto window_size = get_window()->getSize();
	const int window_width = window_size.x;
	const int window_height = window_size.y;

	gui->add_owner_layer(BACKDROP_LAYER, 0, 0, window_width, window_height, resize_fullscreen, draw_splash_backdrop);
	gui->add_layer(LOG_LAYER, 0, 0, window_width, window_height, game_config.gui_font, resize_fullscreen, false);
	splash_loader_started.store(false);
	splash_loader_complete.store(false);	
}

void splash_screen::destroy() {
	gui->delete_layer(LOG_LAYER);
	gui->delete_layer(BACKDROP_LAYER);
}
