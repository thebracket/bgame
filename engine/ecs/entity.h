#pragma once

#include <vector>
#include <bitset>
#include <utility>
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
      case viewshed	: return 3;
      case calendar	: return 4;
    }
}
  
class entity {
public:
    int handle;
    vector<std::pair<component_type,int>> component_handles;
    bitset<64> component_types;
    
    int find_component_by_type(const component_type &type) const {
	int flag = component_flag(type);
	if (!component_types.test(flag)) return -1;
	for (const auto &c : component_handles) {
	    if (c.first == type) return c.second;
	}
	return -1;
    }
};

}
}
