#pragma once

#include <string>
#include <functional>

struct graphviz_t; // Forward
struct raw_species_t; // Forward
struct civilization_t; // Forward

/*
 * Retrieve a species by tag.
 */
const raw_species_t * get_species_def(const std::string &tag) noexcept;

/*
 * Iterate civilization types
 */
void each_civilization_def(const std::function<void(std::string, civilization_t *)> &func) noexcept;

/*
 * Retrieve a civilization definition
 */
civilization_t * get_civ_def(const std::string &tag) noexcept;

/*
 * Retrieve the size of the species list.
 */
std::size_t get_species_defs_size() noexcept;

/*
 * Retrieve the nth species in the list (used in popgen).
 */
std::string get_species_nth_tag(const int &n) noexcept;

/*
 * Linter for the loader.
 */
void sanity_check_species() noexcept ;

/*
 * Lua loader, used in raws loader.
 */
void read_species_types() noexcept;

/*
 * Build the tech tree graphic
 */
void make_civ_tree(graphviz_t * tree);
