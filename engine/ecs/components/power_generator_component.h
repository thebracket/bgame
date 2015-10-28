#pragma once

#include "component_types.h"
#include "../../raws/raw_power_generator_condition.h"

using namespace engine::ecs::serialization_generic;

namespace engine {
namespace ecs {

struct power_generator_component {
     int entity_id;
     int handle;
     component_type type = power_generator;

     power_generator_component() {}
     power_generator_component ( const int &n, const engine::raws::power_generator_condition &c ) : amount ( n ), generator_mode ( c ) {}

     int amount;
     engine::raws::power_generator_condition generator_mode;

     void save ( fstream &lbfile ) {
          save_common_component_properties<power_generator_component> ( lbfile, *this );
          save_primitive<int> ( lbfile, amount );
          save_primitive<int> ( lbfile, generator_mode );
     }

     void load ( fstream &lbfile ) {
          load_common_component_properties<power_generator_component> ( lbfile, *this );
          load_primitive<int> ( lbfile, amount );
          int n = 0;
          load_primitive ( lbfile, n );
          generator_mode = static_cast<engine::raws::power_generator_condition> ( n );
     }
};

}
}