#pragma once
#include <vector>
#include <unordered_map>
#include "../components/species.hpp"
#include "../bengine/color_t.hpp"

struct civ_t {
	std::string species_tag = "";
	uint8_t tech_level = 0;
	bool extinct = false;
	std::string name = "";
	std::string leader_name = "";
	std::string origin = "";
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint16_t glyph = 0;
	int startx = 0;
	int starty = 0;
	bool met_cordex = false;
	int cordex_feelings = 0;
	std::unordered_map<std::size_t, int> relations;

	// For types that have it...
	std::pair<std::string, bengine::color_t> hair_color;
	hair_style_t hair_style;
	std::pair<std::string, bengine::color_t> skin_color;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(species_tag, tech_level, extinct, name, leader_name, r, g, b, glyph, origin, met_cordex, relations,
			cordex_feelings);
	}
};

struct region_info_t {
	std::size_t owner_civ = 0;
	uint8_t blight_level = 0;
	uint8_t settlement_size = 0;
	std::vector<std::string> improvements;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(owner_civ, blight_level, improvements);
	}
};

struct unit_t {
	std::size_t owner_civ = 0;
	uint16_t world_x = 0;
	uint16_t world_y = 0;
	std::string unit_type = "";
	bool dead = false;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(owner_civ, world_x, world_y, unit_type);
	}
};

struct civ_holder_t {
	std::vector<civ_t> civs;
	std::vector<unit_t> units;
	std::vector<region_info_t> region_info;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(civs, units, region_info);
	}
};
