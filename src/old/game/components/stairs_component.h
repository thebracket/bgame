#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace serialization_generic;

namespace stairs_types {
constexpr uint8_t DOWN = 0;
constexpr uint8_t UP = 1;
constexpr uint8_t UPDOWN = 2;
}

struct stairs_component
{
	stairs_component()
	{
	}

	stairs_component(const uint8_t &st) {
		stairs_type = st;
	}

	int entity_id;
	int handle;
	component_type type = stairs;
	bool deleted = false;
	uint8_t stairs_type = 0;

	void save(fstream &lbfile)
	{
		save_common_component_properties<stairs_component>(lbfile, *this);
		save_primitive<uint8_t>(lbfile, stairs_type);
	}

	void load(fstream &lbfile)
	{
		load_common_component_properties<stairs_component>(lbfile, *this);
		load_primitive<uint8_t>(lbfile, stairs_type);
	}
};
