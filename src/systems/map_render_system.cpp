#include "map_render_system.hpp"
#include "../raws/raws.hpp"
#include "../messages/messages.hpp"
#include "../game_globals.hpp"
#include <iostream>
#include <boost/container/flat_map.hpp>

using namespace rltk;

boost::container::flat_map<int, rltk::vchar> renderables;

vchar get_render_char(const int &x, const int &y, const int &z) {

	const int max_dive_depth = 5;
	int dive_depth = 0;
	vchar result{' ', rltk::colors::GREY, rltk::colors::BLACK};

	while (dive_depth < max_dive_depth && result.glyph == ' ') {
		const int idx = current_region.idx(x, y, z-dive_depth);

		auto rf = renderables.find(idx);
		if (rf != renderables.end()) {
			result = rf->second;
		} else {
			result = current_region.tiles[idx].render_as;
		}
		++dive_depth;
	}

	if (result.glyph == ' ') {
		return vchar{' ', rltk::colors::GREY, rltk::colors::BLACK};
	} else {
		const int darken_amount = (dive_depth-1) * 40;
		vchar darkened = result;
		darkened.foreground = darken(darken_amount, darkened.foreground);
		darkened.background = darken(darken_amount, darkened.background);
		return darkened;
	}
}

vchar get_render_char_mining(const int &x, const int &y, const int &z) {

	vchar result;

	const int idx = current_region.idx(x, y, z);

	auto rf = renderables.find(idx);
	if (rf != renderables.end()) {
		result = rf->second;
	} else {
		result = current_region.tiles[idx].render_as;
	}

	auto mf = designations->mining.find(idx);
	if (mf != designations->mining.end()) {
		result.foreground = rltk::colors::BLACK;
		result.background = rltk::colors::YELLOW;
		switch (mf->second) {
			case 1 : result.glyph = 177; break;
			case 2 : result.glyph = 31; break;
			case 3 : result.glyph = 30; break;
			case 4 : result.glyph = '<'; break;
			case 5 : result.glyph = '>'; break;
			case 6 : result.glyph = 'X'; break;
		}
	}

	return result;
}

void map_render_system::configure() {
	subscribe<renderables_changed_message>([this](renderables_changed_message &msg) {
		this->renderables_changed = true;
	});
	renderables_changed = true;
	subscribe<map_dirty_message>([this](map_dirty_message &msg) {
		dirty = true;
	});
}

void map_render_system::update(const double duration_ms) {
	if (dirty) update_clipping_rectangle();

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period) && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))) {
		--camera_position->region_z;
		if (camera_position->region_z < 0) camera_position->region_z = 0;
		dirty = true;
		update_clipping_rectangle();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma) && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))) {
		++camera_position->region_z;
		if (camera_position->region_z > REGION_DEPTH) camera_position->region_z = REGION_DEPTH;
		dirty = true;
		update_clipping_rectangle();
	}

	if (clip_left == -1) update_clipping_rectangle();

	if (renderables_changed) {
		renderables.clear();
		each<renderable_t, position_t>([] (entity_t &entity, renderable_t &render, position_t &pos) {
			renderables[current_region.idx(pos.x, pos.y, pos.z)] = rltk::vchar{render.glyph, render.foreground, render.background};
		});
		dirty = true;
	}

	if (dirty) {
		term(1)->clear();
		const int camera_z = camera_position->region_z;

		std::function<vchar(int,int,int)> calculator;
		if (game_master_mode == PLAY) {
			calculator = get_render_char;
		} else {
			calculator = get_render_char_mining;
		}

		int Y = 2;
		for (int y=clip_top; y<clip_bottom; ++y) {
			int X = 0;
			for (int x=clip_left; x<clip_right; ++x) {
				term(1)->set_char(X, Y, calculator(x, y, camera_z));
				++X;
			}
			++Y;
		}

		dirty = false;
		renderables_changed = false;
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

