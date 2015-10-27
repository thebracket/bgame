#pragma once

#include "entity.h"
#include <memory>

using std::unique_ptr;

namespace engine {
namespace ecs {

int next_entity_handle();
  
entity make_test_entity(const int &x, const int &y);
entity make_camera_entity();

}
}
