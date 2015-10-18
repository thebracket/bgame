#pragma once

#include "base_system.h"
#include "../../game/world/world.h"
#include "ecs.h"
#include "debug_name_component.h"
#include <sstream>

using std::stringstream;

namespace engine {
namespace ecs {
  
class test_system : public base_system {
  virtual void tick(const double &duration_ms) {
      stringstream message;
      
      vector<base_component *> debug_names = find_components_by_type(name);
      for (const base_component * c : debug_names) {
	  message << static_cast<const debug_name_component *>(c)->debug_name;
      }
      
      double fps = 1000/duration_ms;
      message << ". FPS: " << fps;
      world::log.write(message.str());
  }
};
  
}
}

