#pragma once

#include "../world/world.h"
#include "../../engine/ecs.hpp"
#include <vector>
#include <unordered_map>
#include <string>

using std::unordered_map;
using std::vector;
using std::string;
using engine::base_system;

class inventory_system : public base_system {
public:
     inventory_system() {
     }

     virtual void tick ( const double &duration_ms );
};
