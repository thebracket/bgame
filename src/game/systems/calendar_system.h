#pragma once

#include "../../engine/ecs.hpp"
#include "../components/calendar_component.h"

using engine::base_system;

constexpr double TICK_LENGTH = 1.0;

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
	void calculate_sun_angle(calendar_component* t) const;
};
