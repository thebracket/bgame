#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace serialization_generic;

struct item_component {
     item_component() {}

     int entity_id;
     int handle;
     component_type type = item;
     bool deleted = false;

     bool claimed = false;

     void save ( fstream &lbfile ) {
          save_common_component_properties<item_component> ( lbfile, *this );
	  save_primitive<bool>( lbfile, claimed );
     }

     void load  ( fstream &lbfile ) {
          load_common_component_properties<item_component> ( lbfile, *this );
	  load_primitive<bool>( lbfile, claimed );
     }
};
