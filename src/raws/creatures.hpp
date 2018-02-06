#pragma once

#include <string>

struct raw_creature_t; // Forward

/*
 * Retrieve a creature definition by tag.
 */
const raw_creature_t * get_creature_def(const std::string &tag) noexcept;

/*
 * Linter for creature definitions.
 */
void sanity_check_creatures() noexcept;

/*
 * Read creatures from Lua.
 */
void read_creature_types() noexcept;
