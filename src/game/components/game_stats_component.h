#pragma once

#include <string>
#include <vector>
#include "component_types.h"

using std::vector;
using std::string;

using namespace serialization_generic;

struct skill_t {
  string skill_name;
  char skill_level;
};

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
     short ethics;
     int age;
     vector<skill_t> skills;
     
     void save ( fstream &lbfile ) {
          save_common_component_properties<game_stats_component> ( lbfile, *this );
          save_primitive<short> ( lbfile, strength );
          save_primitive<short> ( lbfile, dexterity );
          save_primitive<short> ( lbfile, constitution );
          save_primitive<short> ( lbfile, intelligence );
          save_primitive<short> ( lbfile, wisdom );
          save_primitive<short> ( lbfile, charisma );
          save_primitive<short> ( lbfile, comeliness );
	  save_primitive<short> ( lbfile, ethics );
	  save_primitive<int> ( lbfile, age );
	  save_primitive<int>( lbfile, skills.size() );
	  for (const skill_t &skill : skills) {
	      save_primitive<string>( lbfile, skill.skill_name );
	      save_primitive<char>( lbfile, skill.skill_level );
	  }
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
	  load_primitive<short> ( lbfile, ethics );
	  load_primitive<int>( lbfile, age );
	  int number_of_skills;
	  load_primitive<int>( lbfile, number_of_skills );
	  for (int i=0; i<number_of_skills; ++i) {
	      skill_t new_skill;
	      load_primitive<string>( lbfile, new_skill.skill_name );
	      load_primitive<char>( lbfile, new_skill.skill_level );
	  }
     }
};
