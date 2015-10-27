#pragma once

#include "base_component.h"
#include <string>
#include <iostream>

using std::string;

namespace engine {
namespace ecs {

class debug_name_component : public base_component {
public:
    debug_name_component();
    debug_name_component(const string &NAME);
    string debug_name;
    
    virtual void save(fstream &lbfile);
    virtual void load(fstream &lbfile);
};
  
}
}
