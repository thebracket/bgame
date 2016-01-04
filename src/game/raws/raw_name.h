#pragma once

#include <string>
#include "base_raw.h"
#include "../components/debug_name_component.h"
#include "../game.h"

using std::string;

namespace raws {

struct raw_name : public base_raw {
    raw_name() { type = NAME; };
    raw_name(const string &new_name) : name(new_name) { type=NAME; }
    string name;
    
    virtual void build_components(entity &parent, const int &x, const int &y, const bool placeholder=false) const {
	game_engine->ecs->add_component(parent, debug_name_component(name));
    }
    
    virtual string get_name_override() override {
       return name;
     }
};
  
}
