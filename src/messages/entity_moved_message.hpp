#pragma once

#include <rltk.hpp>
#include "../components/position.hpp"

struct entity_moved_message : public rltk::base_message_t {
	entity_moved_message() {}
	entity_moved_message(std::size_t id, const position_t dest) : entity_id(id), destination(dest) {}
	std::size_t entity_id;
	position_t destination;
};
