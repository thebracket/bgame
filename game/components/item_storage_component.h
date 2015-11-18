#pragma once

#include "component_types.h"

using namespace serialization_generic;

struct item_storage_component {
     item_storage_component() {}
     item_storage_component(const int &container) : container_id(container) {}

     int entity_id;
     int handle;
     component_type type = item_storage;
     bool deleted = false;

     int container_id;
     bool claimed = false;

     void save ( fstream &lbfile ) {
          save_common_component_properties<item_storage_component> ( lbfile, *this );
	  save_primitive<int>( lbfile, container_id );
	  save_primitive<bool>( lbfile, claimed );
     }

     void load  ( fstream &lbfile ) {
          load_common_component_properties<item_storage_component> ( lbfile, *this );
	  load_primitive<int>( lbfile, container_id );
	  load_primitive<bool>( lbfile, claimed );
     }
};
