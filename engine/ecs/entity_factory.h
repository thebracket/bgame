#pragma once

#include "entity.h"
#include <memory>

#include "components/position_component.h"
#include "components/debug_name_component.h"
#include "components/renderable_component.h"
#include "components/viewshed_component.h"
#include "components/calendar_component.h"
#include "components/settler_ai_component.h"
#include "components/obstruction_component.h"
#include "components/power_battery_component.h"
#include "components/power_generator_component.h"

using std::unique_ptr;

namespace engine {
namespace ecs {

int next_entity_handle();
  
entity make_test_entity(const int &x, const int &y);
entity make_camera_entity();
entity make_cordex(const int &x, const int &y, const long &system_time);

unique_ptr<base_component> construct_component_from_file(fstream &lbfile);

}
}
