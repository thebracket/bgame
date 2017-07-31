#pragma once

//#include <rltk.hpp>

struct profession_t; // Forward

//profession_t * get_random_profession(rltk::random_number_generator &rng);

/*
 * Lua reader
 */
void read_professions() noexcept;

/*
 * Lua linter
 */
void sanity_check_professions() noexcept;

