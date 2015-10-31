#pragma once

#include "component_types.h"

using std::fstream;
using namespace serialization_generic;

struct calendar_component {
public:
     int entity_id;
     int handle;
     component_type type = calendar;

     calendar_component() {}
     calendar_component ( const long &t ) : system_time ( t ) {}

     long system_time = 0;
     short year = 0;
     char month = 0;
     char day = 0;
     char hour = 0;
     char minute = 0;

     double duration_buffer = 0.0;

     void save ( fstream &lbfile ) {
          save_common_component_properties<calendar_component> ( lbfile, *this );
          save_primitive<long> ( lbfile, system_time );
          save_primitive<short> ( lbfile, year );
          save_primitive<char> ( lbfile, month );
          save_primitive<char> ( lbfile, day );
          save_primitive<char> ( lbfile, hour );
          save_primitive<char> ( lbfile, minute );
     }

     void load ( fstream &lbfile ) {
          load_common_component_properties<calendar_component> ( lbfile, *this );
          load_primitive<long> ( lbfile, system_time );
          load_primitive<short> ( lbfile, year );
          load_primitive<char> ( lbfile, month );
          load_primitive<char> ( lbfile, day );
          load_primitive<char> ( lbfile, hour );
          load_primitive<char> ( lbfile, minute );
     }
};
