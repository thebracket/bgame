#pragma once

#include <string>

struct item_creator_t {

	item_creator_t() = default;
	item_creator_t(const std::size_t id, const std::string name) : creator_id(id), creator_name(name) {}
	std::size_t creator_id = 0;
	std::string creator_name = "";

};

