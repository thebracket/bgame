#pragma once

#include "component_types.h"
#include <vector>
#include <iostream>

using std::vector;
using namespace engine::ecs::serialization_generic;

namespace engine {
namespace ecs {

enum viewshed_type {visibility,penetrating};

struct viewshed_component {
     int entity_id;
     int handle;
     component_type type = viewshed;

     viewshed_component() {}
     viewshed_component ( const viewshed_type &t, const int &r ) : scanner_type ( t ), scanner_range ( r ) {}

     viewshed_type scanner_type;
     int scanner_range;
     vector<int> last_visibility;

     void load ( fstream &lbfile ) {
          load_common_component_properties<viewshed_component> ( lbfile, *this );
          load_primitive<int> ( lbfile, scanner_range );
          load_primitive<viewshed_type> ( lbfile, scanner_type );
     };

     void save ( fstream &lbfile ) {
          save_common_component_properties<viewshed_component> ( lbfile, *this );
          save_primitive<int> ( lbfile, scanner_range );
          save_primitive<viewshed_type> ( lbfile, scanner_type );
     }
};

}
}
