#pragma once

#include "component_types.h"

using namespace serialization_generic;

enum facing_t { OMNI, NORTH, SOUTH, EAST, WEST };

struct position_component {
     int entity_id;
     int handle;
     component_type type = position;
     bool deleted = false;

     position_component() {}
     position_component ( const int &nx, const int &ny ) : x ( nx ), y ( ny ) { facing = NORTH; }
     position_component ( const int &nx, const int &ny, const facing_t face ) : x ( nx ), y ( ny ), facing(face) {}

     int x;
     int y;
     bool moved;
     facing_t facing;

     void load ( fstream &lbfile ) {
          load_common_component_properties<position_component> ( lbfile, *this );
          load_primitive<int> ( lbfile, x );
          load_primitive<int> ( lbfile, y );
	  int f=0;
	  load_primitive<int>( lbfile, f );
	  facing = static_cast<facing_t>(f);
     }

     void save ( fstream &lbfile ) {
          save_common_component_properties<position_component> ( lbfile, *this );
          save_primitive<int> ( lbfile, x );
          save_primitive<int> ( lbfile, y );
	  save_primitive<int> ( lbfile, facing );
     }
};
