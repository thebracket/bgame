#pragma once

#include <string>
#include "base_raw.h"
#include "../virtual_terminal.h"

using std::string;
using engine::vterm::color_t;

namespace engine {
namespace raws {

struct raw_description : public base_raw {
    raw_description() { type = DESCRIPTION; };
    raw_description(const string &desc) : description(desc) { type = DESCRIPTION; };
    string description;
};
  
}
}
