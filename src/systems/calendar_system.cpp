#include "calendar_system.hpp"
#include "../game_globals.hpp"
#include "../messages/messages.hpp"

void calendar_system::configure() {
	system_name = "Calendar";
	subscribe_mbox<key_pressed_t>();
}

void calendar_system::update(const double duration_ms) {
	bool requested_pause = false;
	bool requested_step = false;
	std::queue<key_pressed_t> * messages = mbox<key_pressed_t>();
	while (!messages->empty()) {
		key_pressed_t e = messages->front();
		messages->pop();

		if (e.event.key.code == sf::Keyboard::Space) requested_pause = true;
		if (e.event.key.code == sf::Keyboard::Period && !e.event.key.shift) requested_step = true;
	}

	if (pause_mode != PAUSED) {
		time_count += duration_ms;

		if (time_count > MS_PER_TICK) {
			time_count = 0.0;

			calendar->next_minute();
			emit(tick_message{});

			// Update the calendar display
			term(2)->clear();
			term(2)->print(0,0,calendar->get_date_time(), rltk::colors::GREEN);

			if (pause_mode == ONE_STEP) {
				pause_mode = PAUSED;
			}
		}
	} else {
		term(2)->clear();
		term(2)->print(0,0,calendar->get_date_time() + " PAUSED", rltk::colors::GREEN);
	}

	if (game_master_mode == PLAY) {
		if (requested_pause) {
			if (pause_mode == RUNNING) {
				pause_mode = PAUSED;
			} else {
				pause_mode = RUNNING;
			}
		}
		if (requested_step) {
			pause_mode = ONE_STEP;
		}
	}
}
