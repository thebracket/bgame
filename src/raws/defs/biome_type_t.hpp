#pragma once

#include <vector>
#include <string>
#include "../../bengine/color_t.hpp"

/*
 * Defines a region biome type.
 */
struct biome_type_t {
    std::string name = "";
    int8_t min_rain = 0;
    int8_t max_rain = 100;
    int8_t min_temp = -100;
    int8_t max_temp = 100;
    uint8_t min_mutation = 0;
    uint8_t max_mutation = 100;
    uint8_t soil_pct = 50;
    uint8_t sand_pct = 50;
    std::vector<uint8_t> occurs;
    unsigned int worldgen_texture_index = 0;
    std::vector<std::pair<std::string,int>> plants;
    std::vector<std::string> wildlife;
    int deciduous_tree_chance = 0;
    int evergreen_tree_chance = 0;
    std::vector<std::string> nouns;
};
