#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <boost/optional.hpp>

/*
 * Defines a group of natives who live on the planet.
 */
struct native_population_t {
    std::string name = "";
    short aggression = 0;
    short strength = 0;
    short dexterity = 0;
    short constitution = 0;
    short intelligence = 0;
    short wisdom = 0;
    short charisma = 0;
    short comeliness = 0;
    short ethics = 0;
    std::vector< std::tuple< uint8_t, std::string, std::string >> starting_clothes;
    std::string melee = "";
    std::string ranged = "";
    std::string ammo = "";
};

/*
 * Gets a list of all native types matching a given tag. Returns a REFERENCE.
 */
boost::optional<std::vector<native_population_t> &> get_native_professions(const std::string &tag) noexcept;

/*
 * Load the natives from Lua
 */
void read_native_population_types(std::ofstream &tech_tree_file) noexcept;

/*
 * Sanity check - used in raw loading linter.
 */
void sanity_check_natives() noexcept;
