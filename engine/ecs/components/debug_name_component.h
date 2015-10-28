#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace engine::ecs::serialization_generic;

namespace engine {
namespace ecs {

struct debug_name_component {
     debug_name_component() {}
     debug_name_component ( const string s ) : debug_name ( s ) {}

     int entity_id;
     int handle;
     component_type type = name;

     string debug_name;

     void save ( fstream &lbfile ) {
          save_common_component_properties<debug_name_component> ( lbfile, *this );
          save_primitive<string> ( lbfile, debug_name );
     }

     void load  ( fstream &lbfile ) {
          load_common_component_properties<debug_name_component> ( lbfile, *this );
          load_primitive<string> ( lbfile, debug_name );
     }
};

}
}
