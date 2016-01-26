#pragma once

#include "component_types.h"

using std::fstream;
using namespace serialization_generic;

struct parent_entity_component
{
public:
	int entity_id;
	int handle;
	component_type type = parent_entity;
	bool deleted = false;
	int parent_id;

	parent_entity_component()
	{
	}

	void save(fstream &lbfile)
	{
		save_common_component_properties<parent_entity_component>(lbfile, *this);
		save_primitive<int>(lbfile, parent_id);
	}

	void load(fstream &lbfile)
	{
		load_common_component_properties<parent_entity_component>(lbfile, *this);
		load_primitive<int>(lbfile, parent_id);
	}
};
