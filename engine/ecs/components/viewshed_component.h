#pragma once

#include "base_component.h"
#include <vector>

using std::vector;

namespace engine {
namespace ecs {

enum viewshed_type {visibility,penetrating};
  
class viewshed_component : public base_component {
public:
    viewshed_component();
    viewshed_component(const viewshed_type &T, const int &R);

    viewshed_type scanner_type;
    int scanner_range;
    vector<int> last_visibility;
    
    virtual void save(fstream &lbfile);
    virtual void load(fstream &lbfile);
};

}
}
