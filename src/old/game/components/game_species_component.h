#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace serialization_generic;

namespace gender_t
{
const short MALE = 1;
const short FEMALE = 2;
const short HERMAPHRODITE = 3;
const short ASEXUAL = 4;
}

namespace preference_t
{
const short HETEROSEXUAL = 1;
const short HOMOSEXUAL = 2;
const short BISEXUAL = 3;
const short ASEXUAL = 4;
}

namespace hair_color_t
{
const char WHITE = 1;
const char BROWN = 2;
const char BLACK = 3;
const char BLONDE = 4;
}

namespace hair_style_t
{
const char BALD = 1;
const char SHORT = 2;
const char LONG = 3;
const char PIGTAILS = 4;
const char MOHAWK = 5;
const char BALDING = 6;
const char TRIANGLE = 7;
}

namespace skin_color_t
{
const char CAUCASIAN = 1;
const char ASIAN = 2;
const char INDIAN = 3;
const char AFRICAN = 4;
}

struct game_species_component
{
	game_species_component()
	{
	}

	int entity_id;
	int handle;
	component_type type = gamespecies;
	bool deleted = false;

	string species;
	short gender;
	short sexual_preference;
	short height_cm;
	short weight_kg;
	char hair_color;
	char hair_style;
	char skin_color;
	bool bearded;

	void save(fstream &lbfile)
	{
		save_common_component_properties<game_species_component>(lbfile, *this);
		save_primitive<string>(lbfile, species);
		save_primitive<short>(lbfile, gender);
		save_primitive<short>(lbfile, sexual_preference);
		save_primitive<short>(lbfile, height_cm);
		save_primitive<short>(lbfile, weight_kg);
		save_primitive<char>(lbfile, hair_color);
		save_primitive<char>(lbfile, hair_style);
		save_primitive<char>(lbfile, skin_color);
		save_primitive<bool>(lbfile, bearded);
	}

	void load(fstream &lbfile)
	{
		load_common_component_properties<game_species_component>(lbfile, *this);
		load_primitive<string>(lbfile, species);
		load_primitive<short>(lbfile, gender);
		load_primitive<short>(lbfile, sexual_preference);
		load_primitive<short>(lbfile, height_cm);
		load_primitive<short>(lbfile, weight_kg);
		load_primitive<char>(lbfile, hair_color);
		load_primitive<char>(lbfile, hair_style);
		load_primitive<char>(lbfile, skin_color);
		load_primitive<bool>(lbfile, bearded);
	}
};
