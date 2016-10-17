#pragma once

#include <istream>
#include <ostream>
#include <vector>
#include <rltk.hpp>
#include "../../raws/reaction_input.hpp"

struct building_designation_t {
	int x,y,z;
	std::vector<std::pair<std::size_t, bool>> component_ids;

	std::string name;
	std::string tag;
	std::vector<reaction_input_t> components;
    int width;
    int height;
    std::vector<rltk::vchar> glyphs;
	std::size_t building_entity = 0;

	void save(std::ostream &f);
    void load(std::istream &f);
};
