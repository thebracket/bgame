#pragma once

#include "component_types.h"

using namespace serialization_generic;

struct item_carried_component {
     item_carried_component() {}

     int entity_id;
     int handle;
     component_type type = item_carried;

     int carried_by_id;
     int carried_position;
     bool equipped = false;
     bool deleted = false;

     void save ( fstream &lbfile ) {
          save_common_component_properties<item_carried_component> ( lbfile, *this );
	  save_primitive<int>( lbfile, carried_by_id );
	  save_primitive<int>( lbfile, carried_position );
	  save_primitive<bool>( lbfile, equipped );
     }

     void load  ( fstream &lbfile ) {
          load_common_component_properties<item_carried_component> ( lbfile, *this );
	  load_primitive<int>( lbfile, carried_by_id );
	  load_primitive<int>( lbfile, carried_position );
	  load_primitive<bool>( lbfile, equipped );
     }
};
