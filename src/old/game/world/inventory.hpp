#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "location.hpp"

struct available_item_t
{
	std::string name;
	location_t location;
	int entity_id;
};

extern std::unordered_map<std::string, std::vector<available_item_t>> inventory;
