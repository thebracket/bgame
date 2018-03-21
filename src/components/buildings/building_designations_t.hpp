#pragma once

#include "../helpers/building_designation_t.hpp"
#include <vector>
#include <utility>
#include <string>
#include <map>

struct building_designations_t {

	std::vector<building_designation_t> buildings;
	std::vector<std::pair<int, std::string>> build_orders;
	std::map<std::string, std::pair<int, std::string>> standing_build_orders; // Item, <#/Reaction>

};
