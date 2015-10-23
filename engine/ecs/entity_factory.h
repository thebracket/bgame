#pragma once

#include "entity.h"
#include <memory>

using std::unique_ptr;

namespace engine {
namespace ecs {

entity make_test_entity(const int &x, const int &y);
entity make_camera_entity();
entity make_cordex(const int &x, const int &y, const long &system_time);

unique_ptr<base_component> construct_component_from_file(fstream &lbfile);

}
}
