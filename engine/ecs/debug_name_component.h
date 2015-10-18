#pragma once

#include "base_component.h"
#include <string>

using std::string;

namespace engine {
namespace ecs {

struct debug_name_component : public base_component {
    debug_name_component() : type(name);
    debug_name_component(const string *NAME) : name(NAME), type(name) {};
    string name;
};
  
}
}