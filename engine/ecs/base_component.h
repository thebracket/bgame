#pragma once

#include <vector>
#include <fstream>

using std::vector;
using std::fstream;

namespace engine {
namespace ecs {

enum component_type{position,name,renderable,viewshed,calendar,settler_ai,obstruction};

class base_component {
public:
    int entity_id;
    int handle;
    component_type type;
    
    virtual void save(fstream &lbfile);
    virtual void load(fstream &lbfile);
};

}
}
