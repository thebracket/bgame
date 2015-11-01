#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace serialization_generic;

struct provisions_component {

     int entity_id;
     int handle;
     component_type type = name;

     char provided_resource; // 1 = food, 2 = water
     char provides_quantity;
     char power_drain;
     string action_name;

     void save ( fstream &lbfile ) {
          save_common_component_properties<provisions_component> ( lbfile, *this );
          save_primitive<char> ( lbfile, provided_resource );
          save_primitive<short> ( lbfile, provides_quantity );
          save_primitive<short> ( lbfile, power_drain );
	  save_primitive<string>( lbfile, action_name );
     }

     void load  ( fstream &lbfile ) {
          load_common_component_properties<provisions_component> ( lbfile, *this );
          load_primitive<char> ( lbfile, provided_resource );
	  load_primitive<char>( lbfile, provides_quantity );
	  load_primitive<char>( lbfile, power_drain );
	  load_primitive<string>( lbfile, action_name );
     }
};
