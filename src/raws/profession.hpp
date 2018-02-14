#pragma once

#include "../bengine/random_number_generator.hpp"

struct profession_t; // Forward

profession_t * get_random_profession(bengine::random_number_generator &rng) noexcept;

/*
 * Lua reader
 */
void read_professions() noexcept;

/*
 * Lua linter
 */
void sanity_check_professions() noexcept;

