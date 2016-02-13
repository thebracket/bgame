#pragma once

#include "../../engine/ecs.hpp"
#include "../components/position_component3d.hpp"
#include "../components/viewshed_component.h"
#include "../../engine/geometry.hpp"
#include <unordered_set>

using geometry::DEGRAD;
using std::unordered_set;
using engine::base_system;

class viewshed_system: public base_system
{
public:
	viewshed_system()
	{
		system_name = "Viewshed System";
	}
	virtual void tick(const double &duration_ms);

private:
	void reset_visibility();
	void scan_radius_for_visibility(viewshed_component * view,
			const position_component3d * pos);
	void scan_radius_penetrating(viewshed_component * view,
			const position_component3d * pos);
	bool is_facing_this_way(const position_component3d * pos,
			const double &angle);
};
