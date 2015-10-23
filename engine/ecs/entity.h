#pragma once

#include <vector>
#include <bitset>
#include <utility>
#include <fstream>
#include "base_component.h"

using std::vector;
using std::bitset;
using std::fstream;

namespace engine {
namespace ecs {

inline int component_flag(const component_type &t) {
    switch (t) {
      case position 	: return 0;
      case name 	: return 1;
      case renderable	: return 2;
      case viewshed	: return 3;
      case calendar	: return 4;
      case settler_ai	: return 5;
    }
}
  
class entity {
public:
    int handle;
    vector<std::pair<component_type,int>> component_handles;
    bitset<64> component_types;
    
    int find_component_by_type(const component_type &type) const;
    void save(fstream &lbfile);
    void load(fstream &lbfile);
};

entity construct_entity_from_file(fstream &lbfile);

}
}
