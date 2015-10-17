#pragma once

#include <unordered_map>
#include "entity.h"
#include "base_component.h"
#include "base_system.h"

using std::unordered_map;

namespace engine {
namespace ecs {

// TODO: Some actual useful code...
void tick(const double duration_ms);

}
}
