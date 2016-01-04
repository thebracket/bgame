#pragma once

#include <iostream>
#include "base_raw.h"
#include "../components/walkable_roof_component.h"
#include "../game.h"

using std::vector;

namespace raws {

struct raw_walkable_roof : public base_raw {
     raw_walkable_roof() {
          type = WALKABLE_ROOF;
     };     
     
     virtual void build_components(entity &parent, const int &x, const int &y, const bool placeholder=false) const override {
	if (placeholder) return;
	walkable_roof_component obs = walkable_roof_component();
	ECS->add_component(parent, std::move(obs));
    }
};

}
