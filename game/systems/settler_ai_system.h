#pragma once

#include "../world/world.h"
#include "../../engine/ecs.hpp"
#include "../components/calendar_component.h"
#include "../components/settler_ai_component.h"
#include "../components/position_component.h"
#include "../../engine/rng.h"
#include <sstream>
#include <string>

using std::string;
using std::stringstream;
using engine::base_system;

class settler_ai_system : public base_system {
     virtual void tick ( const double &duration_ms );
};
