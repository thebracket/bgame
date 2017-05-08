#pragma once

#include <string>
#include <rltk.hpp>
#include <fstream>
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
    std::vector<uint16_t> glyphs;
    std::vector<rltk::vchar> glyphs_ascii;
    std::vector<std::string> provides;
    std::bitset<2> tags;
};

/*
 * Retrieve the index of a plant by tag.
 */
std::size_t get_plant_idx(const std::string &tag) noexcept;

/*
 * Retrieve a plant definition by index.
 */
plant_t& get_plant_def(const std::size_t &index) noexcept;

/*
 * Retrieve all plants.
 */
std::vector<plant_t>& get_plant_defs() noexcept;

/*
 * Run linter on plant definitions
 */
void sanity_check_plants() noexcept;

/*
 * Load plant definitions from Lua
 */
void read_plant_types() noexcept;