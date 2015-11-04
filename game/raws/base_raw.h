#pragma once

#include <vector>
#include <memory>
#include "../../engine/entity.h"
#include "../../engine/ecs.h"

using std::vector;
using std::unique_ptr;

namespace raws {

enum tag_type { RENDERABLE, NAME, DESCRIPTION, GLYPH, COLOR_PAIR, OBSTRUCTION, POWER_GENERATOR, POWER_BATTERY, SETTLER_ACTION, BUILDABLE, SKILL_REQUIRED, COMPONENT, SLEEPABLE };
  
struct base_raw {
    tag_type type;
    vector<unique_ptr<base_raw>> children;
    
    virtual void build_components(entity &parent, const int &x, const int &y) const {
	for (const unique_ptr<base_raw> &child : children) {
	    child->build_components(parent, x, y);
	}
    }
};
  
}
