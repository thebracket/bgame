#pragma once

#include <string>

struct plant_t; // Forward

/*
 * Retrieve the index of a plant by tag.
 */
std::size_t get_plant_idx(const std::string &tag) noexcept;

/*
 * Retrieve a plant definition by index.
 */
plant_t * get_plant_def(const std::size_t &index) noexcept;

/*
 * Run linter on plant definitions
 */
void sanity_check_plants() noexcept;

/*
 * Load plant definitions from Lua
 */
void read_plant_types() noexcept;
