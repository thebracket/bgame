#include "panel_render_system.hpp"
#include "../game_globals.hpp"
#include "../raws/raws.hpp"
#include <sstream>

using namespace rltk;
using namespace rltk::colors;

const color_t GREEN_BG{0,32,0};

void panel_render_system::update(const double duration_ms) {
	term(3)->clear(vchar{' ', WHITE, GREEN_BG});
	term(3)->box(DARKEST_GREEN);

	// Mode switch controls
	if (game_master_mode == PLAY) {
		term(3)->print(1,1,"PLAY", WHITE, DARKEST_GREEN);
		render_play_mode();
	} else {
		term(3)->print(1,1,"PLAY (ESC)", GREEN, GREEN_BG);
	}

	if (game_master_mode == DESIGN) {
		term(3)->print(13,1,"DESIGN", WHITE, DARKEST_GREEN);
		render_design_mode();
	} else {
		term(3)->print(10,1,"(D)ESIGN", GREEN, GREEN_BG);
	}

	if (game_master_mode == PLAY) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			game_master_mode = DESIGN;
			pause_mode = PAUSED;
		}
	}
	if (game_master_mode == DESIGN) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			game_master_mode = PLAY;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			game_design_mode = DIGGING;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
			game_design_mode = BUILDING;
		}
	}
}

void panel_render_system::configure() {

}

void panel_render_system::render_play_mode() {
	// Controls Help
	if (pause_mode == PAUSED) {
		term(3)->print(1,3,"SPACE : Unpause", GREEN, GREEN_BG);
	} else {
		term(3)->print(1,3,"SPACE : Pause", GREEN, GREEN_BG);
	}
	term(3)->print(1,4,".     : One Step", GREEN, GREEN_BG);

	// Mouse tips
	int mouse_x, mouse_y;
	std::tie(mouse_x, mouse_y) = get_mouse_position();

	// Since we're using an 8x8, it's just a matter of dividing by 8 to find the terminal-character
	// coordinates. There will be a helper function for this once we get into retained GUIs.
	const int terminal_x = mouse_x / 8;
	const int terminal_y = mouse_y / 8;

	if (terminal_x >= 0 && terminal_x < term(1)->term_width && terminal_y >= 0 && terminal_y < term(1)->term_height) {
		const int world_x = std::min(clip_left + terminal_x, REGION_WIDTH);
		const int world_y = std::min(clip_top + terminal_y-2, REGION_HEIGHT);
		const int idx = current_region.idx(world_x, world_y, camera_position->region_z);

		{
			const int base_tile_type = current_region.tiles[idx].base_type;
			std::stringstream ss;
			auto finder = tile_types.find(base_tile_type);
			if (finder != tile_types.end()) {
				ss << finder->second.name;
				term(3)->print(1, term(3)->term_height - 2, ss.str(), GREEN, GREEN_BG);
			}
		}
		{
			const int base_tile_content = current_region.tiles[idx].contents;
			std::stringstream ss;
			auto finder = tile_contents.find(base_tile_content);
			if (finder != tile_contents.end()) {
				ss << finder->second.name;
				term(3)->print(1, term(3)->term_height - 3, ss.str(), GREEN, GREEN_BG);
			}
		}
		{
			std::stringstream ss;
			if (current_region.tiles[idx].flags.test(tile_flags::SOLID)) ss << "Solid ";
			if (current_region.tiles[idx].flags.test(tile_flags::TREE)) ss << "Tree ";
			if (current_region.tiles[idx].flags.test(tile_flags::CONSTRUCTION)) ss << "Construct ";
			term(3)->print(1, term(3)->term_height - 4, ss.str(), GREEN, GREEN_BG);
		}
	}
}

void panel_render_system::render_design_mode() {
	if (game_design_mode == DIGGING) {
		term(3)->print(1,3, "Digging", WHITE, DARKEST_GREEN);
	} else {
		term(3)->print(1,3, "(D)igging", GREEN, GREEN_BG);
	}

	if (game_design_mode == BUILDING) {
		term(3)->print(1,4, "Building", WHITE, DARKEST_GREEN);
	} else {
		term(3)->print(1,4, "(B)uilding", GREEN, GREEN_BG);
	}
}