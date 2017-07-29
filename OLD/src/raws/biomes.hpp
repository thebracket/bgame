#pragma once

#include <string>
#include <functional>

struct biome_type_t;

/*
 * Retrieve a biome type by index
 */
biome_type_t * get_biome_def(const std::size_t &index);

/*
 * Lint the biomes list
 */
void sanity_check_biomes() noexcept;

/*
 * Load the biomes from Lua
 */
void read_biome_types() noexcept;

/*
 * Iterate all biomes
 */
void each_biome(std::function<void(biome_type_t *)> func);
