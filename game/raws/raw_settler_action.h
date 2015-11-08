#pragma once

#include <iostream>
#include "base_raw.h"
#include "../components/provisions_component.h"
#include "raw_glyph.h"
#include "raw_color_pair.h"
#include "../game.h"

using std::vector;

namespace raws {

struct raw_settler_action : public base_raw {
     raw_settler_action() {
          type = SETTLER_ACTION;
     };
     
     std::string action_name;
     std::string provides;
     int provides_qty;
     int power_drain;

     virtual void build_components ( entity &parent, const int &x, const int &y ) const {
	provisions_component p;
	p.entity_id = parent.handle;
	p.power_drain = power_drain;
	p.provides_quantity = provides_qty;
	p.provided_resource = 0;
	if (provides == "THIRST") p.provided_resource = 2;
	if (provides == "CALORIES") p.provided_resource = 1;
	p.action_name = action_name;
	
	game_engine->ecs->add_component<provisions_component>(parent, p);
     }
};

}
