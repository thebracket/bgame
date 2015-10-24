#pragma once

#include "base_component.h"
#include <vector>

using std::vector;

namespace engine {
namespace ecs {

enum viewshed_type {visibility,penetrating};
  
class viewshed_component : public base_component {
public:
    viewshed_component() {
        type = viewshed;
    };
    viewshed_component(const viewshed_type &T, const int &R) : scanner_type(T), scanner_range(R) { type = viewshed; };

    viewshed_type scanner_type;
    int scanner_range;
    vector<int> last_visibility;
    
    virtual void save(fstream &lbfile) {
	base_component::save(lbfile);
	lbfile.write ( reinterpret_cast<const char *> ( &scanner_type ), sizeof ( scanner_type ) );
	lbfile.write ( reinterpret_cast<const char *> ( &scanner_range ), sizeof ( scanner_range ) );
    };
    
    virtual void load(fstream &lbfile) {
	base_component::load(lbfile);
	lbfile.read ( reinterpret_cast<char *> ( &scanner_type ), sizeof ( scanner_type ) );
	lbfile.read ( reinterpret_cast<char *> ( &scanner_range ), sizeof ( scanner_range ) );
	last_visibility.clear();
    };
};

}
}
