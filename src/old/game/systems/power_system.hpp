#pragma once

#include "../../engine/ecs.hpp"

using engine::base_system;

class power_system: public base_system
{
public:
	power_system()
	{
		system_name = "Power System";
	}
	virtual void tick(const double &duration_ms);
private:
	long last_tick = 0;
};
