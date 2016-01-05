#pragma once

#include "../world/world.h"
#include "../../engine/ecs.hpp"
#include "../components/calendar_component.h"

using engine::base_system;

constexpr double TICK_LENGTH = 10.0;

class calendar_system: public base_system
{
public:
	calendar_system()
	{
		system_name = "Calendar System";
	}
	virtual void tick(const double &duration_ms);
private:
	void advance_calendar(calendar_component* time);
	void update_display_time(calendar_component* t);
	float calculate_sun_angle(const calendar_component* t) const;
};
