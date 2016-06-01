#pragma once

#include <rltk.hpp>

struct inventory_changed_message : public rltk::base_message_t {
};

struct item_claimed_message : public rltk::base_message_t {
	bool claimed;
	std::size_t id;
};
