#pragma once

#include <string>
#include "base_raw.h"
#include "../components/obstruction_component.h"
#include "../game.h"

using std::string;

namespace raws {

struct raw_obstruction : public base_raw {
    raw_obstruction() { type = OBSTRUCTION; };
    raw_obstruction(const bool &view, const bool &walk) : blocks_view(view),blocks_walk(walk) { type=OBSTRUCTION; }
    bool blocks_view;
    bool blocks_walk;
    
    virtual void build_components(entity &parent, const int &x, const int &y, const bool placeholder=false) const {
	if (placeholder) return;
	obstruction_component obs = obstruction_component();
	obs.blocks_entry = blocks_walk;
	obs.blocks_visibility = blocks_view;
	game_engine->ecs->add_component(parent, std::move(obs));
    }
};
  
}
