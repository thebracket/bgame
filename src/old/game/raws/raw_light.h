#pragma once

#include <iostream>
#include "base_raw.h"
#include "../game.h"
#include "../components/point_light_component.h"

using std::vector;

namespace raws
{

struct raw_light: public base_raw
{
	raw_light(int range)
	{
		type = LIGHT;
		radius = range;
	}
	int radius = 8;

	virtual void build_components(entity &parent, const int &x, const int &y, const bool placeholder = false) const override
	{
		if (!placeholder) {
			point_light_component light(radius);
			ECS->add_component(parent, std::move(light) );
		}
	}
};

}
