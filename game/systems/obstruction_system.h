#pragma once

#include "../../engine/ecs.h"


class obstruction_system : public base_system {
     virtual void tick ( const double &duration_ms );
};
