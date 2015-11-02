#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace serialization_generic;

struct provisions_component {

     int entity_id;
     int handle;
     component_type type = provision;

     int provided_resource; // 1 = food, 2 = water
     int provides_quantity;
     int power_drain;
     string action_name;

     void save ( fstream &lbfile ) {
          save_common_component_properties<provisions_component> ( lbfile, *this );
          save_primitive<int> ( lbfile, provided_resource );
          save_primitive<int> ( lbfile, provides_quantity );
          save_primitive<int> ( lbfile, power_drain );
	  save_primitive<string>( lbfile, action_name );
     }

     void load  ( fstream &lbfile ) {
          load_common_component_properties<provisions_component> ( lbfile, *this );
          load_primitive<int> ( lbfile, provided_resource );
	  load_primitive<int>( lbfile, provides_quantity );
	  load_primitive<int>( lbfile, power_drain );
	  load_primitive<string>( lbfile, action_name );
     }
};
