#include "camera_system.hpp"
#include "../messages/map_dirty_message.hpp"

void camera_system::configure() {
	system_name = "Camera";
	dirty = true;
	subscribe_mbox<key_pressed_t>();
	subscribe_mbox<map_dirty_message>();
}

void camera_system::update(const double duration_ms) {
	std::queue<map_dirty_message> * map_change = mbox<map_dirty_message>();
	while (!map_change->empty()) {
		dirty = true;
		map_change->pop();
	}

	// Handle camera controls
	std::queue<key_pressed_t> * messages = mbox<key_pressed_t>();
	while (!messages->empty()) {
		key_pressed_t e = messages->front();
		messages->pop();

		if (e.event.key.code == sf::Keyboard::Left) {
			--camera_position->region_x;
			if (camera_position->region_x < 0) camera_position->region_x = 0;
			dirty = true;
			update_clipping_rectangle();
		}
		if (e.event.key.code == sf::Keyboard::Right) {
			++camera_position->region_x;
			if (camera_position->region_x > REGION_WIDTH) camera_position->region_x = REGION_WIDTH;
			dirty = true;
			update_clipping_rectangle();
		}
		if (e.event.key.code == sf::Keyboard::Up) {
			--camera_position->region_y;
			if (camera_position->region_y < 0) camera_position->region_y = 0;
			dirty = true;
			update_clipping_rectangle();
		}
		if (e.event.key.code == sf::Keyboard::Down) {
			++camera_position->region_y;
			if (camera_position->region_y > REGION_HEIGHT) camera_position->region_y = REGION_HEIGHT;
			dirty = true;
			update_clipping_rectangle();
		}
		if (e.event.key.code == sf::Keyboard::Period && e.event.key.shift) {
			--camera_position->region_z;
			if (camera_position->region_z < 0) camera_position->region_z = 0;
			dirty = true;
			update_clipping_rectangle();
		}
		if (e.event.key.code == sf::Keyboard::Comma && e.event.key.shift) {
			++camera_position->region_z;
			if (camera_position->region_z > REGION_DEPTH) camera_position->region_z = REGION_DEPTH;
			dirty = true;
			update_clipping_rectangle();
		}
	}

	if (dirty) update_clipping_rectangle();
	if (clip_left == -1) update_clipping_rectangle();
}

void camera_system::update_clipping_rectangle() {
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

	emit(map_rerender_message{});
}
