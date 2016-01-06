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
	point_light_component(const int &rng, float r=1.0, float g=1.0, float b=1.0) :
		light_range(rng), red(r), green(g), blue(b)
	{
	}

	int light_range;
	vector<int> last_visibility;
	float red;
	float green;
	float blue;

	void load(fstream &lbfile)
	{
		load_common_component_properties<point_light_component>(lbfile, *this);
		load_primitive<int>(lbfile, light_range);
		load_primitive<float>(lbfile, red);
		load_primitive<float>(lbfile, green);
		load_primitive<float>(lbfile, blue);
	}
	;

	void save(fstream &lbfile)
	{
		save_common_component_properties<point_light_component>(lbfile, *this);
		save_primitive<int>(lbfile, light_range);
		save_primitive<float>(lbfile, red);
		save_primitive<float>(lbfile, green);
		save_primitive<float>(lbfile, blue);
	}
};
