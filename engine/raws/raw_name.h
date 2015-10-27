#pragma once

#include <string>
#include "base_raw.h"
#include "../ecs/components/debug_name_component.h"

using std::string;

namespace engine {
namespace raws {

struct raw_name : public base_raw {
    raw_name() { type = NAME; };
    raw_name(const string &new_name) : name(new_name) { type=NAME; }
    string name;
    
    virtual void build_components(engine::ecs::entity &parent, const int &x, const int &y) const {
	engine::ecs::add_component(parent, ecs::debug_name_component(name));
    }
};
  
}
}
