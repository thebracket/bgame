#pragma once

#include "../../bengine/rexspeeder.hpp"
#include <string>
#include <vector>
#include <bitset>

constexpr int PLANT_SPREADS = 0;
constexpr int PLANT_ANNUAL = 1;

/*
 * Defines a plant template.
 */
struct plant_t {
    std::string tag = "";
    std::string name = "";
    std::vector<int> lifecycle;
    std::vector<xp::vchar> glyphs_ascii;
    std::vector<std::string> provides;
    std::bitset<2> tags;
	bool requires_light = true;
};

