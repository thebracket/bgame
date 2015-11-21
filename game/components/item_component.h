#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace serialization_generic;

struct item_component {
     item_component() {}
     item_component( const string &t) : item_type(t) {}

     int entity_id;
     int handle;
     component_type type = item;
     bool deleted = false;

     bool claimed = false;
     string item_type;

     void save ( fstream &lbfile ) {
          save_common_component_properties<item_component> ( lbfile, *this );
	  save_primitive<bool>( lbfile, claimed );
	  save_primitive<string>( lbfile, item_type );
     }

     void load  ( fstream &lbfile ) {
          load_common_component_properties<item_component> ( lbfile, *this );
	  load_primitive<bool>( lbfile, claimed );
	  load_primitive<string>( lbfile, item_type );
     }
};
