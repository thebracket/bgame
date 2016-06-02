#pragma once

#include <rltk.hpp>

struct inventory_changed_message : public rltk::base_message_t {
};

struct item_claimed_message : public rltk::base_message_t {
	item_claimed_message() {}
	item_claimed_message(const bool &c, const int &i) : claimed(c), id(i) {}
	bool claimed;
	std::size_t id;
};
