#pragma once

#include "component_types.h"
#include <string>

using namespace engine::ecs::serialization_generic;
using std::string;

namespace engine {
namespace ecs {

struct settler_ai_component {
     int entity_id;
     int handle;
     component_type type = settler_ai;

     int next_tick = 0;
     string first_name;
     string last_name;
     string profession_tag;

     void load ( fstream &lbfile ) {
          load_common_component_properties<settler_ai_component> ( lbfile, *this );
          load_primitive<int> ( lbfile, next_tick );
	  load_primitive<string>(lbfile, first_name);
	  load_primitive<string>(lbfile, last_name);
	  load_primitive<string>(lbfile, profession_tag);
     }

     void save ( fstream &lbfile ) {
          save_common_component_properties<settler_ai_component> ( lbfile, *this );
          save_primitive<int> ( lbfile, next_tick );
	  save_primitive<string> ( lbfile, first_name );
	  save_primitive<string>( lbfile, last_name );
	  save_primitive<string>( lbfile, profession_tag );
     }
};

}
}
