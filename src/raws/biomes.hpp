#pragma once

#include <string>
#include <vector>
#include <rltk.hpp>
#include <utility>
#include <fstream>

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
    uint8_t worldgen_glyph;
    rltk::color_t worldgen_color;
    std::vector<std::pair<std::string,int>> plants;
    std::vector<std::string> wildlife;
    int deciduous_tree_chance = 0;
    int evergreen_tree_chance = 0;
    std::vector<std::string> nouns;
};

/*
 * Retrieve a biome type by index
 */
biome_type_t& get_biome_def(const std::size_t &index);

/*
 * Retrieve all biome types
 */
std::vector<biome_type_t>& get_biome_defs();

/*
 * Lint the biomes list
 */
void sanity_check_biomes() noexcept;

/*
 * Load the biomes from Lua
 */
void read_biome_types(std::ofstream &tech_tree_file) noexcept;