#pragma once

#include "component_types.h"

using std::fstream;
using namespace serialization_generic;

struct particle_emitter_component {
public:
     int entity_id;
     int handle;
     component_type type = particle_emitter;
     bool deleted = false;

     particle_emitter_component() {}
     particle_emitter_component(const string &msg, const int &time, const int &freq, const int col=7) : message(msg), ttl(time), emits_one_in_x(freq), color(col) {}
     
     string message;
     int ttl=64;
     int emits_one_in_x;
     int color;


     void save ( fstream &lbfile ) {
          save_common_component_properties<particle_emitter_component> ( lbfile, *this );
	  save_primitive<int>( lbfile, ttl );
	  save_primitive<string>( lbfile, message );
	  save_primitive<int>( lbfile, emits_one_in_x );
	  save_primitive<int>( lbfile, color );
     }

     void load ( fstream &lbfile ) {
          load_common_component_properties<particle_emitter_component> ( lbfile, *this );
	  load_primitive<int>( lbfile, ttl );
	  load_primitive<string>( lbfile, message );
	  load_primitive<int>( lbfile, emits_one_in_x );
	  load_primitive<int>( lbfile, color );
     }
};
