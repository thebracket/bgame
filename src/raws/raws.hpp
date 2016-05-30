#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>
#include <rltk.hpp>

struct string_table_t {
	std::vector<std::string> strings;

	inline std::string random_entry(rltk::random_number_generator rng=rltk::random_number_generator()) {
		const int position = rng.roll_dice(1, strings.size()) - 1;
		return strings[position];
	}
};

struct tile_type_t {
	std::string name = "";
	int index;
	uint8_t glyph;
	rltk::color_t fg;
	rltk::color_t bg;
};

struct tile_content_t {
	std::string name = "";
	int index;
	uint8_t glyph;
	rltk::color_t fg;
	rltk::color_t bg;
	uint8_t stairs = 0;
};

struct clothing_t {
	std::string name= "";
	std::vector<std::string> colors;
	std::string slot;
	std::string description;
};

struct profession_t {
	std::string name = "";	
	short strength = 0;
	short dexterity = 0;
	short constitution = 0;
	short intelligence = 0;
	short wisdom = 0;
	short charisma = 0;
	short comeliness = 0;
	short ethics;
	std::vector< std::tuple< uint8_t, std::string, std::string >> starting_clothes;
};

extern string_table_t first_names_male;
extern string_table_t first_names_female;
extern string_table_t last_names;

extern std::unordered_map<int, tile_type_t> tile_types;
extern std::unordered_map<std::string, int> tile_type_index;
extern std::unordered_map<int, tile_content_t> tile_contents;
extern std::unordered_map<std::string, int> tile_contents_index;

extern std::unordered_map<std::string, clothing_t> clothing_types;
extern std::vector<profession_t> starting_professions;

void load_raws();
int get_tile_type_index(const std::string name);
int get_tile_contents_index(const std::string name);
