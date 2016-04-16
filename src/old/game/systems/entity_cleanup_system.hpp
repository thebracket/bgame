#pragma once

#include "../../engine/ecs.hpp"

using engine::base_system;

class entity_cleanup_system: public base_system
{
public:
	entity_cleanup_system()
	{
		system_name = "Cleanup System";
	}
	virtual void tick(const double &duration_ms);
};
