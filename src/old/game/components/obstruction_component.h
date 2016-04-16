#pragma once

#include "component_types.h"

using namespace serialization_generic;
struct obstruction_component
{
	int entity_id;
	int handle;
	component_type type = obstruction;
	bool deleted = false;

	obstruction_component()
	{
	}
	obstruction_component(const bool &entry, const bool &visibility) :
			blocks_entry(entry), blocks_visibility(visibility)
	{
	}

	bool blocks_entry;
	bool blocks_visibility;

	void load(fstream &lbfile)
	{
		load_common_component_properties<obstruction_component>(lbfile, *this);
		load_primitive<bool>(lbfile, blocks_entry);
		load_primitive<bool>(lbfile, blocks_visibility);
	}

	void save(fstream &lbfile)
	{
		save_common_component_properties<obstruction_component>(lbfile, *this);
		save_primitive<bool>(lbfile, blocks_entry);
		save_primitive<bool>(lbfile, blocks_visibility);
	}
};
