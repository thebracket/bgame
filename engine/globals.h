#pragma once

#include "../config.h"
#include "ecs/ecs.h"

#include "ecs/components/component_types.h"
#include "ecs/components/calendar_component.h"
#include "ecs/components/debug_name_component.h"
#include "ecs/components/obstruction_component.h"
#include "ecs/components/position_component.h"
#include "ecs/components/power_battery_component.h"
#include "ecs/components/power_generator_component.h"
#include "ecs/components/renderable_component.h"
#include "ecs/components/settler_ai_component.h"
#include "ecs/components/viewshed_component.h"

using engine::entity_component_system;

namespace engine {
namespace globals {
  extern GAME_ECS * ecs;
}
}
