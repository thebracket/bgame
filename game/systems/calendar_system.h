#pragma once

#include "../world/world.h"
#include "../../engine/ecs.h"
#include "../components/calendar_component.h"

constexpr double TICK_LENGTH = 1.0;

class calendar_system : public base_system {
public:
     virtual void tick ( const double &duration_ms );
private:
     void advance_calendar( calendar_component* time );
     void update_display_time( const calendar_component* t );
     float calculate_sun_angle(const calendar_component* t) const;
};
