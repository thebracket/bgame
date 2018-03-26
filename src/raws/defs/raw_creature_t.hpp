#pragma once

#include <string>
#include <vector>
#include "../../bengine/color_t.hpp"
#include <unordered_map>

/*
 * Defines a creature's built-in attack type.
 */
struct creature_attack_t {
    std::string type = "";
    int hit_bonus = 0;
    int damage_n_dice = 1;
    int damage_dice = 6;
    int damage_mod = 0;
};

/*
 * What type of critter AI should the creature use?
 */
enum creature_ai_t { creature_grazer };

/*
 * Defines a creature template.
 */
struct raw_creature_t {
    std::string tag = "";
    std::string name = "";
    std::string male_name = "";
    std::string female_name = "";
    std::string collective_name = "";
    std::string description = "";
    std::unordered_map<std::string,int> stats;
    std::vector<std::tuple<std::string, int, int>> body_parts;
    int armor_class=10;
    std::vector<creature_attack_t> attacks;
    int yield_hide=0;
    int yield_meat=0;
    int yield_bone=0;
    int yield_skull=0;
    creature_ai_t ai;
    uint16_t glyph;
    uint16_t glyph_ascii;
	int vox = 0;
    bengine::color_t fg;
    int hp_n, hp_dice, hp_mod;
    int group_size_n_dice, group_size_dice, group_size_mod;
};
