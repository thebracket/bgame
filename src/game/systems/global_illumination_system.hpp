#pragma once

#include "../../engine/ecs.hpp"
#include "../components/calendar_component.h"

using engine::base_system;

class global_illumination_system: public base_system
{
public:
	global_illumination_system()
	{
		system_name = "Global Illumination";
	}
	virtual void tick(const double &duration_ms);
private:
	bool has_run = false;
};
