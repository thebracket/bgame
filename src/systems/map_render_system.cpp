#include "map_render_system.hpp"
#include "../game_globals.hpp"
#include <iostream>

using namespace rltk;

void map_render_system::update(const double duration_ms) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		--camera_position->region_x;
		if (camera_position->region_x < 0) camera_position->region_x = 0;
		dirty = true;
		update_clipping_rectangle();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		++camera_position->region_x;
		if (camera_position->region_x > REGION_WIDTH) camera_position->region_x = REGION_WIDTH;
		dirty = true;
		update_clipping_rectangle();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		--camera_position->region_y;
		if (camera_position->region_y < 0) camera_position->region_y = 0;
		dirty = true;
		update_clipping_rectangle();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		++camera_position->region_y;
		if (camera_position->region_y > REGION_HEIGHT) camera_position->region_y = REGION_HEIGHT;
		dirty = true;
		update_clipping_rectangle();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period)) {
		--camera_position->region_z;
		if (camera_position->region_z < 0) camera_position->region_z = 0;
		dirty = true;
		update_clipping_rectangle();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma)) {
		++camera_position->region_z;
		if (camera_position->region_z > REGION_DEPTH) camera_position->region_z = REGION_DEPTH;
		dirty = true;
		update_clipping_rectangle();
	}

	if (clip_left == -1) update_clipping_rectangle();

	if (dirty) {
		term(1)->clear();

		int Y = 2;
		for (int y=clip_top; y<clip_bottom; ++y) {
			int X = 0;
			for (int x=clip_left; x<clip_right; ++x) {
				const int map_idx = current_region.idx(x, y, camera_position->region_z);
				term(1)->set_char(X, Y, get_render_char(map_idx));

				++X;
			}
			++Y;
		}

		term(1)->print(0,1,"Press Q to quit");
		dirty = false;
	}
}

void map_render_system::update_clipping_rectangle() {
	const int term_width = term(1)->term_width;
	const int term_height = term(1)->term_height;
	const int half_width = term_width / 2;
	const int half_height = term_height / 2;

	clip_left = camera_position->region_x - half_width;
	clip_right = camera_position->region_x + half_width;
	clip_top = camera_position->region_y - half_height -2;
	clip_bottom = camera_position->region_y + half_height -2;

	if (clip_left < 0) clip_left = 0;
	if (clip_right > REGION_WIDTH) clip_right = REGION_WIDTH;
	if (clip_top < 0) clip_top = 0;
	if (clip_bottom > REGION_HEIGHT) clip_bottom = REGION_HEIGHT;
}

vchar map_render_system::get_render_char_for_base(const uint8_t base_type) const {
	switch (base_type) {
		case tile_type::NOTHING : return vchar{' ', rltk::colors::GREY, rltk::colors::BLACK};
		case tile_type::SEMI_MOLTEN_ROCK : return vchar{177, rltk::colors::RED, rltk::colors::YELLOW};
		case tile_type::ROCK : return vchar{219, rltk::colors::GREY, rltk::colors::BLACK};
		case tile_type::SOIL : return vchar{176, rltk::colors::Brown, rltk::colors::BLACK};
		case tile_type::YELLOW_SAND : return vchar{176, rltk::colors::YELLOW, rltk::colors::BLACK};
		case tile_type::RED_SAND : return vchar{176, rltk::colors::RED, rltk::colors::BLACK};
		default : return vchar{'?', rltk::colors::MAGENTA, rltk::colors::BLACK};
	}
}

vchar map_render_system::get_render_char(const int idx) const {
	if (current_region.tiles[idx].flags.test(tile_flags::SOLID) && !current_region.tiles[idx].flags.test(tile_flags::CONSTRUCTION)) {
		return get_render_char_for_base(current_region.tiles[idx].base_type);
	} else {
		switch (current_region.tiles[idx].contents) {
			case tile_content::NOTHING : return get_render_char_for_base(current_region.tiles[idx].base_type);
			case tile_content::PERMAFROST_WHITE : return vchar{178, rltk::colors::WHITE, rltk::colors::BLACK};
			case tile_content::PERMAFROST_BLUE : return vchar{178, rltk::colors::CYAN, rltk::colors::BLACK};
			case tile_content::PERMAFROST_CRACKED : return vchar{178, rltk::colors::WHITE, rltk::colors::BLACK};
			case tile_content::PERMAFROST_DIRTY : return vchar{178, rltk::colors::GREY, rltk::colors::BLACK};
			case tile_content::YELLOW_SAND : return vchar{176, rltk::colors::YELLOW, rltk::colors::BLACK};
			case tile_content::YELLOW_SAND_CRACKED : return vchar{176, rltk::colors::YELLOW, rltk::colors::BLACK};
			case tile_content::YELLOW_SAND_DIRTY : return vchar{177, rltk::colors::YELLOW, rltk::colors::BLACK};
			case tile_content::YELLOW_SAND_RIPPLES : return vchar{247, rltk::colors::YELLOW, rltk::colors::BLACK};
			case tile_content::RED_SAND : return vchar{176, rltk::colors::RED, rltk::colors::BLACK};
			case tile_content::RED_SAND_CRACKED : return vchar{178, rltk::colors::RED, rltk::colors::BLACK};
			case tile_content::RED_SAND_DIRTY : return vchar{177, rltk::colors::DARK_RED, rltk::colors::BLACK};
			case tile_content::RED_SAND_RIPPLES : return vchar{247, rltk::colors::RED, rltk::colors::BLACK};

			case tile_content::PEBBLES : return vchar{176, rltk::colors::LIGHT_GREY, rltk::colors::BLACK};
			case tile_content::WHITE_SAND : return vchar{176, rltk::colors::LIGHTEST_GREY, rltk::colors::BLACK};
			case tile_content::GRAVEL : return vchar{178, rltk::colors::GREY, rltk::colors::BLACK};
			case tile_content::MUD : return vchar{178, rltk::colors::Brown, rltk::colors::BLACK};
			case tile_content::LYCHEN : return vchar{'%', rltk::colors::GREEN, rltk::colors::BLACK};
			case tile_content::MOSS : return vchar{'#', rltk::colors::GREEN, rltk::colors::BLACK};
			case tile_content::CACTUS : return vchar{157, rltk::colors::GREEN, rltk::colors::BLACK};
			case tile_content::GRASS : return vchar{219, rltk::colors::GREEN, rltk::colors::BLACK};
			case tile_content::GRASS_SPARSE : return vchar{176, rltk::colors::GREEN, rltk::colors::BLACK};
			case tile_content::REEDS : return vchar{'&', rltk::colors::GREEN, rltk::colors::BLACK};
			case tile_content::GORSE : return vchar{'^', rltk::colors::GREEN, rltk::colors::BLACK};
			case tile_content::WILDFLOWER : return vchar{'^', rltk::colors::YELLOW, rltk::colors::BLACK};
			case tile_content::THISTLE : return vchar{'!', rltk::colors::GREEN, rltk::colors::BLACK};
			case tile_content::SHRUB : return vchar{'*', rltk::colors::GREEN, rltk::colors::BLACK};
			case tile_content::HEATHER : return vchar{'#', rltk::colors::GREEN, rltk::colors::BLACK};

			case tile_content::CORDEX : return vchar{15, rltk::colors::CYAN, rltk::colors::BLACK};
			case tile_content::SCANNER_CONSOLE : return vchar{17, rltk::colors::CYAN, rltk::colors::BLACK};
			case tile_content::DEFENSE_CONSOLE : return vchar{16, rltk::colors::CYAN, rltk::colors::BLACK};
			case tile_content::EDUCATION_CONSOLE : return vchar{30, rltk::colors::CYAN, rltk::colors::BLACK};
			case tile_content::COMMUNICATIONS_CONSOLE : return vchar{31, rltk::colors::CYAN, rltk::colors::BLACK};
			case tile_content::SHIP_FLOOR : return vchar{176, rltk::colors::GREY, rltk::colors::DARK_GREY};
			case tile_content::SHIP_WALL_EW : return vchar{205, rltk::colors::WHITE, rltk::colors::BLACK};
			case tile_content::SHIP_WALL_NS : return vchar{186, rltk::colors::WHITE, rltk::colors::BLACK};
			case tile_content::SHIP_WALL_SE : return vchar{201, rltk::colors::WHITE, rltk::colors::BLACK};
			case tile_content::SHIP_WALL_NE : return vchar{200, rltk::colors::WHITE, rltk::colors::BLACK};
			case tile_content::SHIP_WALL_NW : return vchar{187, rltk::colors::WHITE, rltk::colors::BLACK};
			case tile_content::SHIP_WALL_SW : return vchar{188, rltk::colors::WHITE, rltk::colors::BLACK};
			case tile_content::SHIP_WALL : return vchar{219, rltk::colors::WHITE, rltk::colors::BLACK};
			case tile_content::SHIP_UPDOWN : return vchar{'X', rltk::colors::GREY, rltk::colors::BLACK};
			case tile_content::SHIP_UP : return vchar{'<', rltk::colors::GREY, rltk::colors::BLACK};
			case tile_content::SHIP_DOWN : return vchar{'>', rltk::colors::GREY, rltk::colors::BLACK};
			case tile_content::SOLAR_PANEL : return vchar{176, rltk::colors::YELLOW, rltk::colors::CYAN};
			case tile_content::CRYO_BED : return vchar{'0', rltk::colors::WHITE, rltk::colors::BLACK};
			case tile_content::STORAGE_LOCKER : return vchar{236, rltk::colors::WHITE, rltk::colors::BLACK};
			case tile_content::BATTERY : return vchar{241, rltk::colors::LIGHT_RED, rltk::colors::BLACK};
			case tile_content::RTG : return vchar{206, rltk::colors::ORANGE, rltk::colors::BLACK};
			case tile_content::REPLICATOR : return vchar{228, rltk::colors::MAGENTA, rltk::colors::BLACK};

			default : return vchar{'?', rltk::colors::MAGENTA, rltk::colors::BLACK};
		}
	}
	return vchar{' ', rltk::colors::GREY, rltk::colors::BLACK};
}
