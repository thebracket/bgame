#pragma once

#include "base_raw.h"
#include "raw_power_generator_condition.h"

namespace raws {

struct raw_power_generator : public base_raw {
    raw_power_generator() { type = POWER_GENERATOR; };
    raw_power_generator(const power_generator_condition &cond, const int &makes) : condition(cond),amount(makes) { type=POWER_GENERATOR; };
    power_generator_condition condition;
    int amount;
    
    virtual void build_components(entity &parent, const int &x, const int &y, const bool placeholder=false) const;
};
  
}
