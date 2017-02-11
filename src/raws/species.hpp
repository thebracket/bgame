#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <fstream>

/*
 * Defines species dietary preferences
 */
enum diet_t { diet_omnivore, diet_herbivore, diet_carnivore };

/*
 * Defines base species alignment. This will be replaced at some point.
 */
enum alignment_t { align_good, align_neutral, align_evil };

struct ethics_t {
    std::string diet;
    std::string mentality;
    bool spread_blight = false;
    std::string behavior;
};

struct natural_attack_t {
    std::string type;
    int hit_bonus = 0;
    int hit_dice = 0;
    int die_type = 0;
    int n_dice = 0;
    int die_mod = 0;

};

struct civ_combat_t {
    int base_armor_class = 10;
    std::vector<natural_attack_t> natural_attacks;
};

struct caste_t {
    std::string tag = "";
    std::string name_override = "";
    bool fertile = true;
    int hp_n = 0;
    int hp_dice = 0;
    int hp_mod = 0;
    int max_per_region = 0;
    int min_per_occupied_region = 0;
    int starting_level = 1;
    civ_combat_t combat;
    bool guard_only = false;
    bool spreads_blight = false;
    bool destroys_everything = false;
};

struct raw_civilized_t {
    std::string tag = "";
    std::string name = "";
    std::string male_name = "";
    std::string female_name = "";
    std::string collective_name = "";
    std::string baby_name = "";
    std::string description = "";
    std::unordered_map<std::string,int> stat_mods;
    std::vector<std::tuple<std::string, int, int>> body_parts;
    ethics_t ethics;
    int max_age = 90;
    int infant_age = 5;
    int child_age = 12;
    uint16_t glyph = '@';
    std::unordered_map<std::string, caste_t> castes;
    std::string breed_type = "";
    int clutch_size_min=0;
    int clutch_size_max=0;
    int clutch_frequency=0;
    bool gains_tech_by_eating=false;
    bool lisp=false;
    bool never_negotiates=false;
    diet_t diet = diet_omnivore;
};

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
    int max_age = 90;
    int infant_age = 5;
    int child_age = 12;
    uint8_t glyph = '@';
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

extern std::unordered_map<std::string, raw_civilized_t> civ_defs;