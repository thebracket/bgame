#pragma once

#include "../world/world.h"
#include "../../engine/ecs.h"
#include "../components/position_component.h"
#include "../components/viewshed_component.h"
#include "../world/geometry.h"
#include <unordered_set>

using geometry::DEGRAD;
using geometry::line_func;
using geometry::project_angle;
using std::unordered_set;

class viewshed_system : public base_system {
    virtual void tick ( const double &duration_ms );

private:
    void reset_visibility();
    void scan_radius_for_visibility(viewshed_component * view, const position_component * pos);
    void scan_radius_penetrating(viewshed_component * view, const position_component * pos);
    bool is_facing_this_way(const position_component * pos, const double &angle);
};
