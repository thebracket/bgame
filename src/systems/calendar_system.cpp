#include "calendar_system.hpp"
#include "../game_globals.hpp"
#include "../messages/messages.hpp"

void calendar_system::configure() {
	system_name = "Calendar";

	subscribe<pause_requested_message>([this] (pause_requested_message &msg) { requested_pause = true; });
	subscribe<one_step_requested_message>([this] (one_step_requested_message &msg) { requested_step = true; });
}

void calendar_system::update(const double duration_ms) {
	if (pause_mode != PAUSED) {
		time_count += duration_ms;

		if (time_count > MS_PER_TICK) {
			time_count = 0.0;

			auto hour = calendar->hour;
			calendar->next_minute();
			emit(tick_message{});
			if (calendar->hour != hour) emit(hour_elapsed_message{});

			if (pause_mode == ONE_STEP) {
				pause_mode = PAUSED;
			}
		}
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

	requested_pause = false;
	requested_step = false;
}
