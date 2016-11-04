#pragma once

#include <rltk.hpp>
#include <sstream>
#include <iomanip>

using namespace rltk;

enum gender_t { MALE, FEMALE, HERMAPHRODITE };
enum sexuality_t { HETEROSEXUAL, HOMOSEXUAL, BISEXUAL, ASEXUAL };
enum hair_color_t { WHITE_HAIR, BROWN_HAIR, BLACK_HAIR, BLONDE_HAIR, RED_HAIR };
enum hair_style_t { BALD, SHORT, LONG, PIGTAILS, MOHAWK, BALDING, TRIANGLE };
enum skin_color_t { CAUCASIAN, ASIAN, INDIAN, AFRICAN };

struct species_t {
	std::string tag = "";
	gender_t gender;
	sexuality_t sexuality;
	hair_color_t hair_color;
	hair_style_t hair_style;
	skin_color_t skin_color;
	float height_cm;
	float weight_kg;
	bool bearded;

	species_t() {}

	std::string gender_str();
	std::string gender_pronoun();
	std::string sexuality_str();
	std::string height_feet();
	std::string weight_lbs();
	std::string ethnicity();
	std::string hair_color_str();
	std::string hair_style_str();
	
	std::size_t serialization_identity = 6;

	void save(std::ostream &lbfile);
	static species_t load(std::istream &lbfile);
};