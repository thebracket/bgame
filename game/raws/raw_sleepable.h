#pragma once

#include <iostream>
#include "base_raw.h"
#include "../components/provisions_component.h"
#include "raw_glyph.h"
#include "raw_color_pair.h"
#include "../game.h"

using std::vector;

namespace raws {

struct raw_sleepable : public base_raw {
     raw_sleepable() {
          type = SLEEPABLE;
     };
     
     virtual void build_components ( entity &parent, const int &x, const int &y, const bool placeholder=false ) const {
        if (placeholder) return;
	provisions_component p;
	p.entity_id = parent.handle;
	p.power_drain = 0;
	p.provides_quantity = 0;
	p.provided_resource = 3;
	p.action_name = "sleeps";
	
	game_engine->ecs->add_component<provisions_component>(parent, p);
     }
};

}
