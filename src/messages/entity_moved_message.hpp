#pragma once

#include <rltk.hpp>
#include "../components/position.hpp"

struct entity_wants_to_move_randomly_message : public rltk::base_message_t {
	entity_wants_to_move_randomly_message() {}
	entity_wants_to_move_randomly_message(std::size_t id) : entity_id(id) {}
	std::size_t entity_id;
};

struct entity_wants_to_move_message : public rltk::base_message_t {
	entity_wants_to_move_message() {}
	entity_wants_to_move_message(std::size_t id, const position_t dest) : entity_id(id), destination(dest) {}
	std::size_t entity_id;
	position_t destination;
};

struct entity_moved_message : public rltk::base_message_t {
	entity_moved_message() {}
	entity_moved_message(std::size_t id, const position_t &orig, const position_t &dest) : entity_id(id), destination(dest), origin(orig) {}
	std::size_t entity_id;
	position_t origin;
	position_t destination;
};
