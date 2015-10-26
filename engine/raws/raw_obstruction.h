#pragma once

#include <string>
#include "base_raw.h"
#include "../ecs/obstruction_component.h"
#include <memory>

using std::string;
using std::unique_ptr;
using std::make_unique;

namespace engine {
namespace raws {

struct raw_obstruction : public base_raw {
    raw_obstruction() { type = OBSTRUCTION; };
    raw_obstruction(const bool &view, const bool &walk) : blocks_view(view),blocks_walk(walk) { type=OBSTRUCTION; }
    bool blocks_view;
    bool blocks_walk;
    
    virtual void build_components(engine::ecs::entity &parent, const int &x, const int &y) const {
	unique_ptr<ecs::obstruction_component> obs = make_unique<ecs::obstruction_component>();
	obs->blocks_entry = blocks_walk;
	obs->blocks_visibility = blocks_view;
	engine::ecs::add_component(parent, std::move(obs));
    }
};
  
}
}