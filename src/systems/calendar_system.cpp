#include "calendar_system.hpp"
#include "../game_globals.hpp"

void calendar_system::update(const double duration_ms) {
	time_count += duration_ms;

	if (time_count > MS_PER_TICK) {
		time_count = 0.0;

		calendar->next_minute();

		// Emit a tick message

		// Update the calendar display
		term(2)->clear();
		term(2)->print(0,0,calendar->get_date_time());
	}
}