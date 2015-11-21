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

     string debug_name;

     void save ( fstream &lbfile ) {
          save_common_component_properties<item_component> ( lbfile, *this );
     }

     void load  ( fstream &lbfile ) {
          load_common_component_properties<item_component> ( lbfile, *this );
     }
};
