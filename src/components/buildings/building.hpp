#pragma once

#include <vector>
#include "../../bengine/rexspeeder.hpp"

struct building_t {
    building_t(const std::string &ntag, const int w, const int h, const std::vector<xp::vchar> &g,
			   const std::vector<xp::vchar> &ga, const bool comp, const std::size_t owner, const uint8_t maxhp,
				const uint8_t hp, const int vox) :
        tag(ntag), width(w), height(h), glyphs(g), glyphs_ascii(ga), complete(comp), civ_owner(owner),
		max_hit_points(maxhp), hit_points(hp), vox_model(vox)
	{}

    std::string tag;
    int width=0, height=0;
    std::vector<xp::vchar> glyphs;
	std::vector<xp::vchar> glyphs_ascii;
    bool complete = false;
    std::vector<std::pair<std::string, std::size_t>> built_with;
    std::size_t civ_owner=0;
	uint8_t max_hit_points=10;
	uint8_t hit_points=10;
    int vox_model = 0;

	building_t() = default;

};

