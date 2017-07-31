#pragma once

#include <string>

struct clothing_t;

/*
 * Retrieve a clothing template by tag
 */
const clothing_t * get_clothing_by_tag(const std::string &tag) noexcept;

/*
 * Linter for loading
 */
void sanity_check_clothing() noexcept;

/*
 * Lua loader
 */
void read_clothing() noexcept;
