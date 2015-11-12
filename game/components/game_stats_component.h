#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace serialization_generic;

struct game_stats_component {
     game_stats_component() {}

     int entity_id;
     int handle;
     component_type type = gamestats;
     bool deleted = false;

     short strength;
     short dexterity;
     short constitution;
     short intelligence;
     short wisdom;
     short charisma;
     short comeliness;
     int age;
     
     void save ( fstream &lbfile ) {
          save_common_component_properties<game_stats_component> ( lbfile, *this );
          save_primitive<short> ( lbfile, strength );
          save_primitive<short> ( lbfile, dexterity );
          save_primitive<short> ( lbfile, constitution );
          save_primitive<short> ( lbfile, intelligence );
          save_primitive<short> ( lbfile, wisdom );
          save_primitive<short> ( lbfile, charisma );
          save_primitive<short> ( lbfile, comeliness );
	  save_primitive<int> ( lbfile, age );
     }

     void load  ( fstream &lbfile ) {
          load_common_component_properties<game_stats_component> ( lbfile, *this );
          load_primitive<short> ( lbfile, strength );
          load_primitive<short> ( lbfile, dexterity );
          load_primitive<short> ( lbfile, constitution );
          load_primitive<short> ( lbfile, intelligence );
          load_primitive<short> ( lbfile, wisdom );
          load_primitive<short> ( lbfile, charisma );
          load_primitive<short> ( lbfile, comeliness );
	  load_primitive<int>( lbfile, age );
     }
};
