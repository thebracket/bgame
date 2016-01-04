#pragma once

#include <string>
#include "base_raw.h"
#include "../components/description_component.h"
#include "../game.h"

using std::string;

namespace raws {

struct raw_description : public base_raw {
    raw_description() { type = DESCRIPTION; };
    raw_description(const string &desc) : description(desc) { type = DESCRIPTION; };
    string description;
    
    virtual void build_components(entity &parent, const int &x, const int &y, const bool placeholder=false) const {
	game_engine->ecs->add_component(parent, description_component(description));
    }
};
  
}
