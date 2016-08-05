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

struct drop_item_message : public rltk::base_message_t {
	drop_item_message() {}
	drop_item_message(const std::size_t &ID, const int &X, const int &Y, const int &Z) : id(ID), x(X), y(Y), z(Z) {}
	std::size_t id;
	int x,y,z;
};

struct pickup_item_message : public rltk::base_message_t {
	pickup_item_message() {}
	pickup_item_message(const std::size_t &ID, const std::size_t &holder) : id(ID), collector(holder) {}
	std::size_t id;
	std::size_t collector;
};