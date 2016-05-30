#include "calendar_system.hpp"
#include "../game_globals.hpp"
#include "../messages/messages.hpp"

void calendar_system::update(const double duration_ms) {
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			if (pause_mode == RUNNING) {
				pause_mode = PAUSED;
			} else {
				pause_mode = RUNNING;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))) {
			pause_mode = ONE_STEP;
		}
	}
}
