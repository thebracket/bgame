#pragma once

#include "component_types.h"
#include <vector>
#include <iostream>

using std::vector;
using namespace serialization_generic;

struct point_light_component
{
	int entity_id;
	int handle;
	component_type type = pointlight;
	bool deleted = false;

	point_light_component()
	{
	}
	point_light_component(const int &r) :
		light_range(r)
	{
	}

	int light_range;
	vector<int> last_visibility;

	void load(fstream &lbfile)
	{
		load_common_component_properties<point_light_component>(lbfile, *this);
		load_primitive<int>(lbfile, light_range);
	}
	;

	void save(fstream &lbfile)
	{
		save_common_component_properties<point_light_component>(lbfile, *this);
		save_primitive<int>(lbfile, light_range);
	}
};
