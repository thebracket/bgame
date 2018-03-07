#pragma once

#include <istream>
#include <ostream>
#include <vector>
#include "../../raws/reaction_input.hpp"
#include "../../bengine/rexspeeder.hpp"

struct building_designation_t {
	int x,y,z;
	std::vector<std::pair<std::size_t, bool>> component_ids;

	std::string name;
	std::string tag;
	std::vector<reaction_input_t> components;
    int width;
    int height;
    std::vector<xp::vchar> glyphs;
	std::vector<xp::vchar> glyphs_ascii;
	int building_entity = 0;
};
