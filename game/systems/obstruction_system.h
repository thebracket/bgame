#pragma once

#include "../../engine/ecs/ecs.h"

namespace engine {
namespace ecs {

class obstruction_system : public base_system {
     virtual void tick ( const double &duration_ms );
};

}
}
