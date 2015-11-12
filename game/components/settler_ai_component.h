#pragma once

#include "component_types.h"
#include <string>

using namespace serialization_generic;
using std::string;

enum settler_state_t { IDLE, SLEEPING, DRINKING, EATING };

struct settler_ai_component {
     int entity_id;
     int handle;
     component_type type = settler_ai;
     bool deleted = false;

     int next_tick = 0;
     string first_name;
     string last_name;
     string profession_tag;
     int calories;
     int calorie_burn_at_rest;
     int wakefulness;
     int thirst;
     settler_state_t state_major;
     int state_timer;

     void load ( fstream &lbfile ) {
          load_common_component_properties<settler_ai_component> ( lbfile, *this );
          load_primitive<int> ( lbfile, next_tick );
	  load_primitive<string>(lbfile, first_name);
	  load_primitive<string>(lbfile, last_name);
	  load_primitive<string>(lbfile, profession_tag);
	  load_primitive<int>(lbfile, calories);
	  load_primitive<int>(lbfile, calorie_burn_at_rest);
	  load_primitive<int>(lbfile, wakefulness);
	  load_primitive<int>(lbfile, thirst);
	  int state = 0;
	  load_primitive<int>(lbfile, state);
	  state_major = static_cast<settler_state_t>(state);
	  load_primitive<int>(lbfile, state_timer);
     }

     void save ( fstream &lbfile ) {
          save_common_component_properties<settler_ai_component> ( lbfile, *this );
          save_primitive<int> ( lbfile, next_tick );
	  save_primitive<string> ( lbfile, first_name );
	  save_primitive<string>( lbfile, last_name );
	  save_primitive<string>( lbfile, profession_tag );
	  save_primitive<int>( lbfile, calories );
	  save_primitive<int>( lbfile, calorie_burn_at_rest );
	  save_primitive<int>( lbfile, wakefulness );
	  save_primitive<int>( lbfile, thirst );
	  save_primitive<int>( lbfile, state_major );
	  save_primitive<int>( lbfile, state_timer );
     }
};
