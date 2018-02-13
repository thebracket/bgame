#pragma once

#include <iomanip>
#include <string>
#include "../bengine/color_t.hpp"

enum gender_t { MALE, FEMALE };
enum sexuality_t { HETEROSEXUAL, HOMOSEXUAL, BISEXUAL };
enum hair_color_t { WHITE_HAIR, BROWN_HAIR, BLACK_HAIR, BLONDE_HAIR, RED_HAIR };
enum hair_style_t { BALD, SHORT_HAIR, LONG_HAIR, PIGTAILS, MOHAWK, BALDING, TRIANGLE };

struct species_t {
	std::string tag = "";
	std::size_t index = 0;
	gender_t gender;
	sexuality_t sexuality;
	hair_style_t hair_style;
	std::pair<std::string, bengine::color_t> skin_color;
	std::pair<std::string, bengine::color_t> hair_color;
	float height_cm;
	float weight_kg;
	bool bearded;
    uint16_t base_male_glyph;
    uint16_t base_female_glyph;

	std::string gender_str();
	std::string gender_pronoun();
	std::string sexuality_str();
	std::string height_feet();
	std::string weight_lbs();
	std::string ethnicity();
	std::string hair_color_str();
	std::string hair_style_str();	
};

