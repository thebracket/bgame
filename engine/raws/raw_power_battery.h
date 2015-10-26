#pragma once

#include <string>
#include "base_raw.h"
#include "../virtual_terminal.h"

using std::string;
using engine::vterm::color_t;

namespace engine {
namespace raws {

struct raw_power_battery : public base_raw {
    raw_power_battery() { type = POWER_BATTERY; };
    raw_power_battery(const int &capacity) : storage_capacity(capacity) { type = POWER_BATTERY; };
    int storage_capacity;
};
  
}
}
