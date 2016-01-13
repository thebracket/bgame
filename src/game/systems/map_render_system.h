#pragma once

#pragma once

#include "../../engine/ecs.hpp"

using engine::base_system;

class map_render_system: public base_system
{
public:
	map_render_system()
	{
		system_name = "Map Render System";
	}
	virtual void tick(const double &duration_ms);
private:
};
