#pragma once

#include <vector>
#include <functional>

struct biome_type_t;
extern std::vector<std::string> biome_textures;

/*
 * Retrieve a biome type by index
 */
biome_type_t * get_biome_def(const std::size_t &index) noexcept;

/*
 * Lint the biomes list
 */
void sanity_check_biomes() noexcept;

/*
 * Load the biomes from Lua
 */
void read_biome_types() noexcept;

/*
 * Load the biome texture list for displaying world-gen
 */
void read_biome_textures() noexcept;

/*
 * Iterate all biomes
 */
void each_biome(const std::function<void(biome_type_t *)> &func) noexcept;
