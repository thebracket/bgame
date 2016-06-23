#pragma once

#include <rltk.hpp>

using namespace rltk;

enum gender_t { MALE, FEMALE, HERMAPHRODITE };
enum sexuality_t { HETEROSEXUAL, HOMOSEXUAL, BISEXUAL, ASEXUAL };
enum hair_color_t { WHITE_HAIR, BROWN_HAIR, BLACK_HAIR, BLONDE_HAIR };
enum hair_style_t { BALD, SHORT, LONG, PIGTAILS, MOHAWK, BALDING, TRIANGLE };
enum skin_color_t { CAUCASIAN, ASIAN, INDIAN, AFRICAN };

struct species_t {
	gender_t gender;
	sexuality_t sexuality;
	hair_color_t hair_color;
	hair_style_t hair_style;
	skin_color_t skin_color;
	float height_cm;
	float weight_kg;
	bool bearded;

	species_t() {}

	std::string gender_str() {
		switch (gender) {
			case MALE : return "Male";
			case FEMALE : return "Female";
			case HERMAPHRODITE : return "Hermaphrodite";
		}
	}

	std::string gender_pronoun() {
		switch (gender) {
			case MALE : return "He";
			case FEMALE : return "She";
			case HERMAPHRODITE : return "It";
		}
	}

	std::size_t serialization_identity = 6;

	void save(std::ostream &lbfile) {
		serialize(lbfile, gender);
		serialize(lbfile, sexuality);
		serialize(lbfile, hair_color);
		serialize(lbfile, hair_style);
		serialize(lbfile, skin_color);
		serialize(lbfile, height_cm);
		serialize(lbfile, weight_kg);
		serialize(lbfile, bearded);
	}

	static species_t load(std::istream &lbfile) {
		species_t c;
		deserialize(lbfile, c.gender);
		deserialize(lbfile, c.sexuality);
		deserialize(lbfile, c.hair_color);
		deserialize(lbfile, c.hair_style);
		deserialize(lbfile, c.skin_color);
		deserialize(lbfile, c.height_cm);
		deserialize(lbfile, c.weight_kg);
		deserialize(lbfile, c.bearded);
		return c;
	}
};