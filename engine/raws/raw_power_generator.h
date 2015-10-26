#pragma once

#include <string>
#include "base_raw.h"
#include "../virtual_terminal.h"

using std::string;
using engine::vterm::color_t;

namespace engine {
namespace raws {

enum power_generator_condition { DAYLIGHT };
  
struct raw_power_generator : public base_raw {
    raw_power_generator() { type = POWER_GENERATOR; };
    raw_power_generator(const power_generator_condition &cond, const int &makes) : condition(cond),amount(makes) { type=POWER_GENERATOR; };
    power_generator_condition condition;
    int amount;
};
  
}
}