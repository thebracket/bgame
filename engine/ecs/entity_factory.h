#pragma once

#include "entity.h"
#include <memory>

#include "position_component.h"
#include "debug_name_component.h"
#include "renderable_component.h"
#include "viewshed_component.h"
#include "calendar_component.h"
#include "settler_ai_component.h"
#include "obstruction_component.h"

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
