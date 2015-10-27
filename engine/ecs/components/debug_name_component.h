#pragma once

#include <string>
#include "component_types.h"

using std::string;

namespace engine {
namespace ecs {

struct debug_name_component {
    debug_name_component() {}
    debug_name_component(const string s) : debug_name(s) {}
  
    int entity_id;
    int handle;
    component_type type = name;
    
    string debug_name;
};
  
}
}
