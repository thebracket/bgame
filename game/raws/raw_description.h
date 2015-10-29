#pragma once

#include <string>
#include "base_raw.h"

using std::string;

namespace engine {
namespace raws {

struct raw_description : public base_raw {
    raw_description() { type = DESCRIPTION; };
    raw_description(const string &desc) : description(desc) { type = DESCRIPTION; };
    string description;
};
  
}
}
