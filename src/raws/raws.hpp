#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <rltk.hpp>

struct string_table_t {
	std::vector<std::string> strings;

	inline std::string random_entry(rltk::random_number_generator rng=rltk::random_number_generator()) {
		const int position = rng.roll_dice(1, strings.size()) - 1;
		return strings[position];
	}
};

struct tile_type_t {
	std::string name;
	int index;
	uint8_t glyph;
	rltk::color_t fg;
	rltk::color_t bg;
};

struct tile_content_t {
	std::string name;
	int index;
	uint8_t glyph;
	rltk::color_t fg;
	rltk::color_t bg;
};

extern string_table_t first_names_male;
extern string_table_t first_names_female;
extern string_table_t last_names;

extern std::unordered_map<int, tile_type_t> tile_types;
extern std::unordered_map<std::string, int> tile_type_index;
extern std::unordered_map<int, tile_content_t> tile_contents;
extern std::unordered_map<std::string, int> tile_contents_index;

void load_raws();
