#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <rltk.hpp>
#include <boost/optional.hpp>
#include "reaction_input.hpp"

/*
 * Defines a material type. Used by just about everything.
 */
struct material_def_t {
    std::string tag = "";
    std::string name = "";
    material_def_spawn_type_t spawn_type = rock;
    std::string parent_material_tag = "";
    uint8_t glyph;
    rltk::color_t fg;
    rltk::color_t bg;
    uint8_t hit_points = 0;
    std::string mines_to_tag = "";
    std::string mines_to_tag_second = "";
    std::string layer = "";
    std::vector<std::string> ore_materials;
    int damage_bonus = 0;
    float ac_bonus = 0.0F;
};

/*
 * Retrieve a material by tag.
 */
boost::optional<std::size_t> get_material_by_tag(const std::string &tag) noexcept;

/*
 * Retrieve a material by index (it returns a reference; you have the original)
 */
boost::optional<material_def_t &> get_material(const std::size_t &idx) noexcept;

/*
 * Retrieves the material name.
 */
std::string material_name(const std::size_t &id) noexcept;

/*
 * Retrieves the materials that can be applied to different strata. Used in world-gen.
 */
void get_strata_materials(std::vector<std::size_t> &soils, std::vector<std::size_t> &sedimentaries,
                          std::vector<std::size_t> &igneouses, std::vector<std::size_t> &sands) noexcept;

/*
 * Returns true if a material ID is valid.
 */
bool is_material_idx_valid(const std::size_t &id) noexcept;

/*
 * Used in the RAW loader to check that materials make sense.
 */
void sanity_check_materials() noexcept;

/*
 * Reads the material definitions from Lua.
 */
void read_material_types(std::ofstream &tech_tree_file) noexcept;