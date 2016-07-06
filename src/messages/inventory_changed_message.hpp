#pragma once

#include <rltk.hpp>

struct inventory_changed_message : public rltk::base_message_t {
};

struct item_claimed_message : public rltk::base_message_t {
	item_claimed_message() {}
	item_claimed_message(const std::size_t i, const bool c) : claimed(c), id(i) {}
	bool claimed;
	std::size_t id;
};

struct refresh_available_buildings_message : public rltk::base_message_t {
};
