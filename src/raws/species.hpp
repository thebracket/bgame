#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <fstream>
#include "../utils/sparsepp.h"

/*
 * Defines species dietary preferences
 */
enum diet_t { diet_omnivore, diet_herbivore, diet_carnivore };

/*
 * Defines base species alignment. This will be replaced at some point.
 */
enum alignment_t { align_good, align_neutral, align_evil, align_devour };

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
    std::unordered_map<std::string,int> stat_mods;
    std::vector<std::tuple<std::string, int, int>> body_parts;
    diet_t diet = diet_omnivore;
    alignment_t alignment = align_neutral;
    bool spreads_blight = false;
    int max_age = 90;
    int infant_age = 5;
    int child_age = 12;
    uint8_t glyph = '@';
};

/*
 * Definitions of civilizations that can be found.
 */
struct civ_unit_natural_attack_t {
    std::string type = "";
    int hit_bonus = 0;
    int n_dice = 0;
    int die_type = 6;
    int die_mod = 0;
};

struct civ_unit_sentient_t {
    int n_present = 0;
    uint8_t base_level = 0;
    std::string tag = "";
    std::string name = "";
    uint8_t base_armor_class = 10;
    int hp_n = 1;
    int hp_dice = 10;
    int hp_mod = 0;
    std::string gender = "male";
    std::vector<civ_unit_natural_attack_t> natural_attacks;
    // equipment
};

struct civ_unit_t {
    std::string tag = "";
    uint8_t bp_per_turn = 0;
    uint8_t speed = 0;
    std::string name = "";
    std::vector<civ_unit_sentient_t> sentients;
    std::vector<std::string> can_build;
};

struct civilization_t {
    uint8_t tech_level;
    std::string tag;
    std::string species_tag;
    std::string ai;
    std::unordered_map<std::string, civ_unit_t> units;
    std::vector<std::string> evolves_into;
};

/*
 * Retrieve a species by tag.
 */
const raw_species_t * get_species_def(const std::string &tag) noexcept;

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

extern std::unordered_map<std::string, civilization_t> civ_defs;