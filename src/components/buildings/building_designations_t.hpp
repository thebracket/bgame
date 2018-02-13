#pragma once

#include "../helpers/building_designation_t.hpp"
#include <vector>
#include <utility>
#include <string>

struct building_designations_t {

	std::vector<building_designation_t> buildings;
	std::vector<std::pair<uint8_t, std::string>> build_orders;

};
