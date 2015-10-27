#pragma once

#include "base_system.h"
#include "../../../game/world/world.h"
#include "../ecs.h"
#include "../components/calendar_component.h"

namespace engine {
namespace ecs {

const double TICK_LENGTH = 100.0;

class calendar_system : public base_system {
public:
     virtual void tick ( const double &duration_ms );
private:
     void advance_calendar( engine::ecs::calendar_component* time );
     void update_display_time( const engine::ecs::calendar_component* t );
     float calculate_sun_angle(const calendar_component* t) const;
};

}
}
