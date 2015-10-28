#pragma once

#include "component_types.h"

using namespace engine::ecs::serialization_generic;

namespace engine {
namespace ecs {

struct settler_ai_component {
     int entity_id;
     int handle;
     component_type type = settler_ai;

     int next_tick = 0;

     void load ( fstream &lbfile ) {
          load_common_component_properties<settler_ai_component> ( lbfile, *this );
          load_primitive<int> ( lbfile, next_tick );
     }

     void save ( fstream &lbfile ) {
          save_common_component_properties<settler_ai_component> ( lbfile, *this );
          save_primitive<int> ( lbfile, next_tick );
     }
};

}
}
