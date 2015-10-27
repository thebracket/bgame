#pragma once

#include "base_raw.h"
#include <memory>

using std::unique_ptr;
using std::make_unique;

namespace engine {
namespace raws {

enum power_generator_condition { DAYLIGHT };
  
struct raw_power_generator : public base_raw {
    raw_power_generator() { type = POWER_GENERATOR; };
    raw_power_generator(const power_generator_condition &cond, const int &makes) : condition(cond),amount(makes) { type=POWER_GENERATOR; };
    power_generator_condition condition;
    int amount;
    
    virtual void build_components(engine::ecs::entity &parent, const int &x, const int &y) const;
};
  
}
}