#pragma once

#include "base_system.h"
#include "../../../game/world/world.h"
#include "../ecs.h"
#include "../components/calendar_component.h"
#include "../components/settler_ai_component.h"
#include "../components/position_component.h"
#include "../../rng.h"
#include <sstream>
#include <string>

using std::string;
using std::stringstream;

namespace engine {
namespace ecs {


class settler_ai_system : public base_system {
     virtual void tick ( const double &duration_ms );
};

}
}
