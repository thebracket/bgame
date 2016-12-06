#pragma once

#include <string>
#include <vector>
#include <boost/optional.hpp>

/*
 * Defines template for clothes. These need to be merged into items eventually.
 */
struct clothing_t {
    std::string name= "";
    std::vector<std::string> colors;
    std::string slot = "";
    std::string description = "";
    float armor_class = 0.0F;
    uint16_t clothing_glyph = 0;
};

/*
 * Retrieve a clothing template by tag
 */
boost::optional<clothing_t> get_clothing_by_tag(const std::string &tag) noexcept;

/*
 * Linter for loading
 */
void sanity_check_clothing() noexcept;

/*
 * Lua loader
 */
void read_clothing(std::ofstream &tech_tree_file) noexcept;
