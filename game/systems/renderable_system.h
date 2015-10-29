#pragma once

#include "../world/world.h"
#include "../../engine/ecs/ecs.h"
#include "../components/position_component.h"
#include "../components/renderable_component.h"

namespace engine {
namespace ecs {

class renderable_system : public base_system {
     virtual void tick ( const double &duration_ms );
};

}
}
