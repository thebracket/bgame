#pragma once

#include "component_types.h"
#include <vector>

using std::vector;

namespace engine {
namespace ecs {

enum viewshed_type {visibility,penetrating};
  
struct viewshed_component {
    int entity_id;
    int handle;
    component_type type = viewshed; 
    
    viewshed_component() {}
    viewshed_component(const viewshed_type &t, const int &r) : scanner_type(t), scanner_range(r) {}
    
    viewshed_type scanner_type;
    int scanner_range;
    vector<int> last_visibility;
};

}
}
