#pragma once

#include <istream>
#include <ostream>
#include <vector>
#include <rltk.hpp>
#include <cereal/cereal.hpp>
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
	std::vector<rltk::vchar> glyphs_ascii;
	std::size_t building_entity = 0;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( x, y, z, component_ids, name, tag, components, width, height, glyphs, building_entity, glyphs_ascii ); // serialize things by passing them to the archive
	}
};
