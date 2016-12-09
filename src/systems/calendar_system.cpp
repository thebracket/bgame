#include "calendar_system.hpp"
#include "../main/game_globals.hpp"
#include "../messages/messages.hpp"

void calendar_system::configure() {
	system_name = "Calendar";
}

void calendar_system::update(const double duration_ms) {
	if (pause_mode != PAUSED) {
		time_count += duration_ms;

		if (time_count > MS_PER_TICK) {
			time_count = 0.0;

			auto hour = calendar->hour;
			auto day = calendar->day;
			calendar->next_minute();
			emit(tick_message{});
			++slow_tick_count;
			if (slow_tick_count > 9) {
				slow_tick_count = 0;
				emit(slow_tick_message{});
			}
			if (calendar->hour != hour) emit_deferred(hour_elapsed_message{});
			if (calendar->day != day) emit_deferred(day_elapsed_message{});

			if (pause_mode == ONE_STEP) {
				pause_mode = PAUSED;
			}
		}
	}
}
