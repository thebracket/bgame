#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <rltk.hpp>

#include "tile_material.hpp"

struct string_table_t {
	std::vector<std::string> strings;

	inline std::string random_entry(rltk::random_number_generator rng=rltk::random_number_generator()) {
		const int position = rng.roll_dice(1, strings.size()) - 1;
		return strings[position];
	}
};

extern std::unordered_map<std::string, rltk::color_t> color_table;
extern std::unordered_map<std::string, uint8_t> glyph_table;
extern std::unordered_map<int, raw::tile_material> tile_material_table;

void load_raws();
