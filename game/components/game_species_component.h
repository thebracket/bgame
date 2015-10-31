#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace serialization_generic;

namespace gender_t {
const short MALE = 1;
const short FEMALE = 2;
const short HERMAPHRODITE = 3;
const short ASEXUAL = 4;
}

namespace preference_t {
const short HETEROSEXUAL = 1;
const short HOMOSEXUAL = 2;
const short BISEXUAL = 3;
const short ASEXUAL = 4;
}

struct game_species_component {
     game_species_component() {}

     int entity_id;
     int handle;
     component_type type = gamespecies;

     string species;
     short gender;
     short sexual_preference;
     short height_cm;
     short weight_kg;

     void save ( fstream &lbfile ) {
          save_common_component_properties<game_species_component> ( lbfile, *this );
          save_primitive<string> ( lbfile, species );
          save_primitive<short> ( lbfile, gender );
          save_primitive<short> ( lbfile, sexual_preference );
          save_primitive<short> ( lbfile, height_cm );
          save_primitive<short> ( lbfile, weight_kg );
     }

     void load ( fstream &lbfile ) {
          load_common_component_properties<game_species_component> ( lbfile, *this );
          load_primitive<string> ( lbfile, species );
          load_primitive<short> ( lbfile, gender );
          load_primitive<short> ( lbfile, sexual_preference );
          load_primitive<short> ( lbfile, height_cm );
          load_primitive<short> ( lbfile, weight_kg );
     }
};
