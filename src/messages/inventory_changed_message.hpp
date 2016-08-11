#pragma once

#include <rltk.hpp>
#include "../components/item_carried.hpp"

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
	pickup_item_message(const std::size_t &ID, const std::size_t &holder, const item_location_t &LOC) : id(ID), collector(holder), loc(LOC) {}
	std::size_t id;
	std::size_t collector;
	item_location_t loc = INVENTORY;
};

struct destroy_item_message : public rltk::base_message_t {
	destroy_item_message() {}
	destroy_item_message(const std::size_t ID) : id(ID) {}
	std::size_t id;
};
