#include "camera_system.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../messages/inputs.hpp"
#include "renderables_system.hpp"
#include "../messages/tick_message.hpp"

std::vector<int> render_tiles;

uint32_t camera_glyph_cycle = 0;

void camera_system::configure() {
	system_name = "Camera";
	dirty = true;
	subscribe_mbox<camera_move_requested_message>();
	subscribe_mbox<map_dirty_message>();
	subscribe<slow_tick_message>([this](slow_tick_message &msg) {
        ++camera_glyph_cycle;
    });
}

void camera_system::update(const double duration_ms) {
	std::queue<map_dirty_message> * map_change = mbox<map_dirty_message>();
	while (!map_change->empty()) {
		dirty = true;
		map_change->pop();
	}

	// Handle camera controls
	std::queue<camera_move_requested_message> * messages = mbox<camera_move_requested_message>();
	while (!messages->empty()) {
		camera_move_requested_message e = messages->front();
		messages->pop();

		if (e.direction == 1) {
			camera_position->region_x -= e.step;
			if (camera_position->region_x < 0) camera_position->region_x = 0;
			dirty = true;
			update_clipping_rectangle();
		}
		if (e.direction == 2) {
			camera_position->region_x += e.step;
			if (camera_position->region_x > REGION_WIDTH) camera_position->region_x = REGION_WIDTH;
			dirty = true;
			update_clipping_rectangle();
		}
		if (e.direction == 3) {
			camera_position->region_y -= e.step;
			if (camera_position->region_y < 0) camera_position->region_y = 0;
			dirty = true;
			update_clipping_rectangle();
		}
		if (e.direction == 4) {
			camera_position->region_y += e.step;
			if (camera_position->region_y > REGION_HEIGHT) camera_position->region_y = REGION_HEIGHT;
			dirty = true;
			update_clipping_rectangle();
		}
		if (e.direction == 5) {
			camera_position->region_z -= e.step;
			if (camera_position->region_z < 0) camera_position->region_z = 0;
			dirty = true;
			update_clipping_rectangle();
		}
		if (e.direction == 6) {
			camera_position->region_z += e.step;
			if (camera_position->region_z > REGION_DEPTH) camera_position->region_z = REGION_DEPTH;
			dirty = true;
			update_clipping_rectangle();
		}
	}

	if (dirty || clip_left == -1) {
		 update_clipping_rectangle();
		 build_render_tiles();
		 dirty = false;
	}
}

void camera_system::update_clipping_rectangle() {	
	const int term_width = term(1)->term_width;
	const int term_height = term(1)->term_height;
	const int half_width = term_width / 2;
	const int half_height = term_height / 2;

	if (camera_position->region_x < half_width) camera_position->region_x = half_width;
	if (camera_position->region_x > (REGION_WIDTH+2)-half_width) camera_position->region_x = (REGION_WIDTH+2)-half_width;
	if (camera_position->region_y < half_height) camera_position->region_y = half_height;
	if (camera_position->region_y > (REGION_HEIGHT+5)-half_height) camera_position->region_y = (REGION_HEIGHT+5)-half_height;

	clip_left = camera_position->region_x - half_width;
	clip_right = camera_position->region_x + half_width;
	clip_top = camera_position->region_y - half_height -2;
	clip_bottom = camera_position->region_y + half_height -2;

	if (clip_left < 0) clip_left = 0;
	if (clip_right > REGION_WIDTH) clip_right = REGION_WIDTH;
	if (clip_top < 0) clip_top = 0;
	if (clip_bottom > REGION_HEIGHT) clip_bottom = REGION_HEIGHT;

	emit(map_rerender_message{});
}

void camera_system::build_render_tiles() {
	const int term_width = term(1)->term_width;
	const int term_height = term(1)->term_height;
	const int tile_size = term_width * term_height;
	if (render_tiles.size() != tile_size) {
		render_tiles.resize(tile_size);
	}
	std::fill(render_tiles.begin(), render_tiles.end(), 0);

	bool vertical_dive = false;
	if (game_master_mode == PLAY || game_master_mode == ROGUE) vertical_dive = true;

	if (vertical_dive) {
		build_render_tiles_with_vertical_dive();
	} else {
		build_render_tiles_without_vertical_dive();
	}
}

int camera_system::deep_dive(const int &x, const int &y, const int &z) {
	constexpr int max_dive_depth = 5;
	int dive_depth = 0;
	vchar result{' ', rltk::colors::GREY, rltk::colors::BLACK};

	while (dive_depth < max_dive_depth && result.glyph == ' ') {
		const int idx = mapidx(x, y, z-dive_depth);
		//if (!current_region->revealed[idx]) return 0;

		auto rf = renderables.find(idx);
		if (rf != renderables.end()) {
			result = rf->second[camera_glyph_cycle % rf->second.size()];
		} else {
			result = current_region->render_cache[idx];
		}
		++dive_depth;
	}
	--dive_depth;

	if (result.glyph == ' ') {
		return 0;
	} else {
		return mapidx(x,y,z-dive_depth);
	}
}

void camera_system::build_render_tiles_with_vertical_dive() {
	const int z = camera_position->region_z;

	int Y = 0;
	for (int y=clip_top; y<clip_bottom; ++y) {
		int X = 0;
		for (int x=clip_left; x<clip_right; ++x) {
			const int idx = (term(1)->term_width * Y) + X;
			render_tiles[idx] = deep_dive(x,y,z);
			++X;
		}
		++Y;
	}
}

void camera_system::build_render_tiles_without_vertical_dive() {
	const int z = camera_position->region_z;
	int Y = 0;
	for (int y=clip_top; y<clip_bottom; ++y) {
		int X = 0;
		for (int x=clip_left; x<clip_right; ++x) {
			const int idx = (term(1)->term_width * Y) + X;
			render_tiles[idx] = mapidx(x,y,z);
			++X;
		}
		++Y;
	}
}