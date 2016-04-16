#pragma once

#include <iostream>
#include "base_raw.h"
#include "../components/stairs_component.h"
#include "../game.h"

using std::vector;

namespace raws
{

struct raw_stairs: public base_raw
{
	raw_stairs()
	{
		type = STAIRS;
	};

	int stairs_type;

	virtual void build_components(entity &parent, const int &x, const int &y,
			const bool placeholder = false) const override
	{
		if (placeholder) return;

		stairs_component stairs = stairs_component(stairs_type);
		ECS->add_component(parent, std::move(stairs));
	}
};

}
