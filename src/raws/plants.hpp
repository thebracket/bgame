#pragma once

#include <string>
#include <rltk.hpp>
#include <fstream>
#include <vector>
#include <boost/optional.hpp>

/*
 * Defines a plant template.
 */
struct plant_t {
    std::string tag = "";
    std::string name = "";
    uint16_t glyph;
    rltk::color_t fg;
    rltk::color_t bg;
    std::string provides = "";
};

/*
 * Retrieve the index of a plant by tag.
 */
boost::optional<std::size_t> get_plant_idx(const std::string &tag) noexcept;

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
void read_plant_types(std::ofstream &tech_tree_file) noexcept;