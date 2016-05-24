#include "map_render_system.hpp"
#include "../raws/raws.hpp"
#include "../messages/messages.hpp"
#include <iostream>

using namespace rltk;

void map_render_system::configure() {
	subscribe<renderables_changed_message>([this](renderables_changed_message &msg) {
		this->renderables_changed = true;
	});
	renderables_changed = true;
}

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

	if (renderables_changed) {
		for (auto i : renderables) {
			i.reset();
		}
		each<renderable_t, position_t>([this] (entity_t &entity, renderable_t &render, position_t &pos) {
			this->renderables[current_region.idx(pos.x, pos.y, pos.z)] = rltk::vchar{render.glyph, render.foreground, render.background};
		});
	}

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
	if (base_type == 0) return vchar{' ', rltk::colors::GREY, rltk::colors::BLACK};

	auto finder = tile_types.find(base_type);
	if (finder != tile_types.end()) {
		return vchar{ finder->second.glyph, finder->second.fg, finder->second.bg };
	} else {
		return vchar{'?', rltk::colors::MAGENTA, rltk::colors::BLACK};
	}
}

vchar map_render_system::get_render_char(const int idx) const {
	if (renderables[idx]) return renderables[idx].get();

	if (current_region.tiles[idx].flags.test(tile_flags::SOLID) && !current_region.tiles[idx].flags.test(tile_flags::CONSTRUCTION)) {
		return get_render_char_for_base(current_region.tiles[idx].base_type);
	} else {
		if (current_region.tiles[idx].contents == 0) {
			return get_render_char_for_base(current_region.tiles[idx].base_type);
		} else {
			auto finder = tile_contents.find(current_region.tiles[idx].contents);
			if (finder != tile_contents.end()) {
				return vchar{ finder->second.glyph, finder->second.fg, finder->second.bg };
			} else {
				return vchar{'?', rltk::colors::MAGENTA, rltk::colors::BLACK};
			}
		}
	}
	return vchar{' ', rltk::colors::GREY, rltk::colors::BLACK};
}
