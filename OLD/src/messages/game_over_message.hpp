#pragma once

#include <rltk.hpp>

struct game_over_message : public rltk::base_message_t {
	game_over_message() {}
	game_over_message(const int loss) : loss_type(loss) {}

	int loss_type = 1;
};