#pragma once

#include "reaction_input.hpp"
#include "graphviz.hpp"

struct material_def_t;
struct graphviz_t;

extern std::vector<std::string> material_textures;
extern std::vector<std::pair<std::string, std::string>> voxel_models_to_load;

/*
 * Retrieve a material by tag.
 */
std::size_t get_material_by_tag(const std::string &tag) noexcept;

/*
 * Retrieve a material by index (it returns a pointer to the original)
 */
material_def_t * get_material(const std::size_t &idx) noexcept;

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
void read_material_types() noexcept;
void build_material_tech_tree(graphviz_t * tree);
void build_material_acquisition_tech_tree(graphviz_t * tree);

void read_material_textures() noexcept;
void read_voxel_models() noexcept;
