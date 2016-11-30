#pragma once

#include <string>
#include <boost/container/flat_map.hpp>
#include <vector>
#include <tuple>
#include <fstream>
#include <boost/optional.hpp>

/*
 * Defines species dietary preferences
 */
enum diet_t { diet_omnivore, diet_herbivore, diet_carnivore };

/*
 * Defines base species alignment. This will be replaced at some point.
 */
enum alignment_t { align_good, align_neutral, align_evil };

/*
 * Basic definition of a species.
 */
struct raw_species_t {
    std::string tag = "";
    std::string name = "";
    std::string male_name = "";
    std::string female_name = "";
    std::string collective_name = "";
    std::string description = "";
    boost::container::flat_map<std::string,int> stat_mods;
    std::vector<std::tuple<std::string, int, int>> body_parts;
    diet_t diet = diet_omnivore;
    alignment_t alignment = align_neutral;
    int max_age = 90;
    int infant_age = 5;
    int child_age = 12;
    uint8_t glyph = '@';
};

/*
 * Retrieve a species by tag.
 */
boost::optional<raw_species_t> get_species_def(const std::string &tag) noexcept;

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
void read_species_types(std::ofstream &tech_tree_file) noexcept;