#pragma once

#include <vector>
#include <bitset>
#include <utility>
#include <fstream>
#include "components/component_types.h"

using std::vector;
using std::bitset;
using std::fstream;

namespace engine {
namespace ecs {

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
