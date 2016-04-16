#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace serialization_generic;

struct walkable_roof_component
{
	walkable_roof_component()
	{
	}

	int entity_id;
	int handle;
	component_type type = walkable_roof;
	bool deleted = false;

	void save(fstream &lbfile)
	{
		save_common_component_properties<walkable_roof_component>(lbfile,
				*this);
	}

	void load(fstream &lbfile)
	{
		load_common_component_properties<walkable_roof_component>(lbfile,
				*this);
	}
};
