#pragma once

#include "base_component.h"
#include <string>

using std::string;

namespace engine {
namespace ecs {

class debug_name_component : public base_component {
public:
    debug_name_component(const string &NAME) : debug_name(NAME) { type = name; };
    string debug_name;
};
  
}
}
