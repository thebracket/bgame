#pragma once

#include "component_types.h"

using std::fstream;
using namespace serialization_generic;

struct tree_component {
public:
     int entity_id;
     int handle;
     component_type type = tree;
     bool deleted = false;

     tree_component() {}


     void save ( fstream &lbfile ) {
          save_common_component_properties<tree_component> ( lbfile, *this );
     }

     void load ( fstream &lbfile ) {
          load_common_component_properties<tree_component> ( lbfile, *this );
     }
};
