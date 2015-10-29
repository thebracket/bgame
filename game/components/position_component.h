#pragma once

#include "component_types.h"

using namespace engine::ecs::serialization_generic;

namespace engine {
namespace ecs {

struct position_component {
     int entity_id;
     int handle;
     component_type type = position;

     position_component() {}
     position_component ( const int &nx, const int &ny ) : x ( nx ), y ( ny ) {}

     int x;
     int y;
     bool moved;

     void load ( fstream &lbfile ) {
          load_common_component_properties<position_component> ( lbfile, *this );
          load_primitive<int> ( lbfile, x );
          load_primitive<int> ( lbfile, y );
     }

     void save ( fstream &lbfile ) {
          save_common_component_properties<position_component> ( lbfile, *this );
          save_primitive<int> ( lbfile, x );
          save_primitive<int> ( lbfile, y );
     }
};

}
}
