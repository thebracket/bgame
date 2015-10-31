#pragma once

#include "../config.h"
#include "ecs.h"

#include "../game/components/component_types.h"
#include "../game/components/calendar_component.h"
#include "../game/components/debug_name_component.h"
#include "../game/components/obstruction_component.h"
#include "../game/components/position_component.h"
#include "../game/components/power_battery_component.h"
#include "../game/components/power_generator_component.h"
#include "../game/components/renderable_component.h"
#include "../game/components/settler_ai_component.h"
#include "../game/components/viewshed_component.h"
#include "../game/components/game_stats_component.h"
#include "../game/components/game_species_component.h"
#include "../game/components/game_health_component.h"

using engine::entity_component_system;

namespace engine {
namespace globals {
  extern GAME_ECS * ecs;
}
}
