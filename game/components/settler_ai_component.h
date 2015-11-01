#pragma once

#include "component_types.h"
#include <string>

using namespace serialization_generic;
using std::string;

struct settler_ai_component {
     int entity_id;
     int handle;
     component_type type = settler_ai;

     int next_tick = 0;
     string first_name;
     string last_name;
     string profession_tag;
     short calories;
     short wakefulness;
     short thirst;

     void load ( fstream &lbfile ) {
          load_common_component_properties<settler_ai_component> ( lbfile, *this );
          load_primitive<int> ( lbfile, next_tick );
	  load_primitive<string>(lbfile, first_name);
	  load_primitive<string>(lbfile, last_name);
	  load_primitive<string>(lbfile, profession_tag);
	  load_primitive<short>(lbfile, calories);
	  load_primitive<short>(lbfile, wakefulness);
	  load_primitive<short>(lbfile, thirst);
     }

     void save ( fstream &lbfile ) {
          save_common_component_properties<settler_ai_component> ( lbfile, *this );
          save_primitive<int> ( lbfile, next_tick );
	  save_primitive<string> ( lbfile, first_name );
	  save_primitive<string>( lbfile, last_name );
	  save_primitive<string>( lbfile, profession_tag );
	  save_primitive<short>( lbfile, calories );
	  save_primitive<short>( lbfile, wakefulness );
	  save_primitive<short>( lbfile, thirst );
     }
};
