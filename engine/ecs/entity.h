#pragma once

#include <vector>
#include <bitset>
#include "base_component.h"

using std::vector;
using std::bitset;

namespace engine {
namespace ecs {

inline int component_flag(const component_type &t) {
    switch (t) {
      case position 	: return 0;
      case name 	: return 1;
      case renderable	: return 2;
    }
}
  
class entity {
public:
    int handle;
    vector<int> component_handles;
    bitset<64> component_types;
};

}
}
