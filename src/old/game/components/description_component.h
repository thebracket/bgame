#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace serialization_generic;

struct description_component
{
	description_component()
	{
	}
	description_component(const string s) :
			desc(s)
	{
	}

	int entity_id;
	int handle;
	component_type type = description;
	bool deleted = false;

	string desc;

	void save(fstream &lbfile)
	{
		save_common_component_properties<description_component>(lbfile, *this);
		save_primitive<string>(lbfile, desc);
	}

	void load(fstream &lbfile)
	{
		load_common_component_properties<description_component>(lbfile, *this);
		load_primitive<string>(lbfile, desc);
	}
};
