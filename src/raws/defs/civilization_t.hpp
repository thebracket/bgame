#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include "../../bengine/color_t.hpp"

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
    uint16_t glyph = '@';
    uint16_t glyph_ascii = '@';
    uint16_t worldgen_glyph = '@';
    bool render_composite = false;
    uint16_t base_male_glyph = 352;
    uint16_t base_female_glyph = 353;
	int voxel_model = 0;
    std::vector<std::pair<std::string, bengine::color_t>> skin_colors;
    std::vector<std::pair<std::string, bengine::color_t>> hair_colors;
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
    int range = 0;
};

struct civ_equipment_t {
    std::vector< std::tuple< uint8_t, std::string, std::string >> starting_clothes;
    std::string melee = "";
    std::string ranged = "";
    std::string ammo = "";
    std::string mount = "";
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
    civ_equipment_t equipment;
};

struct civ_unit_t {
    std::string tag = "";
    uint8_t bp_per_turn = 0;
    uint8_t speed = 0;
    std::string name = "";
    std::vector<civ_unit_sentient_t> sentients;
    int worldgen_strength = 1;
};

struct civilization_t {
    uint8_t tech_level;
    std::string tag;
    std::string species_tag;
    std::string ai;
    std::string name_generator = "normal";
    std::unordered_map<std::string, civ_unit_t> units;
    std::vector<std::string> evolves_into;
    std::vector<std::string> can_build;
};
