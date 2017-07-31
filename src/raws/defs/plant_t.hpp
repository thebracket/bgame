#pragma once

#include <string>
#include <bitset>
#include <vector>

constexpr int PLANT_SPREADS = 0;
constexpr int PLANT_ANNUAL = 1;

/*
 * Defines a plant template.
 */
struct plant_t {
    std::string tag = "";
    std::string name = "";
    std::vector<int> lifecycle;
    std::vector<uint16_t> glyphs;
    //std::vector<rltk::vchar> glyphs_ascii;
    std::vector<std::string> provides;
    std::bitset<2> tags;
};

