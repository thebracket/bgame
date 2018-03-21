#pragma once

#include <string>
#include <bitset>
#include "../../bengine/color_t.hpp"

constexpr int NUMBER_OF_ITEM_CATEGORIES = 64;
constexpr int COMPONENT = 0;
constexpr int TOOL_CHOPPING = 1;
constexpr int TOOL_DIGGING = 2;
constexpr int WEAPON_MELEE = 3;
constexpr int WEAPON_RANGED = 4;
constexpr int WEAPON_AMMO = 5;
constexpr int ITEM_FOOD = 6;
constexpr int ITEM_SPICE = 7;
constexpr int ITEM_DRINK = 8;
constexpr int ITEM_HIDE = 9;
constexpr int ITEM_BONE = 10;
constexpr int ITEM_SKULL = 11;
constexpr int ITEM_LEATHER = 12;
constexpr int ITEM_FARMING = 13;
constexpr int ITEM_SEED = 14;
constexpr int ITEM_TOPSOIL = 15;
constexpr int ITEM_FERTILIZER = 16;
constexpr int ITEM_FOOD_PREPARED = 17;

struct stockpile_def_t {
    int index = 0;
    std::string name = "";
    std::string tag = "";
};

struct item_def_t {
    std::string tag = "";
    std::string name = "";
    std::string description = "";
    std::bitset<NUMBER_OF_ITEM_CATEGORIES> categories;
    uint16_t glyph;
    uint16_t glyph_ascii = 1;
    bengine::color_t fg;
    bengine::color_t bg;
    int damage_n=0, damage_d=0, damage_mod=0;
    int range = 0;
    std::string ammo;
    int stack_size = 1;
    int initiative_penalty = 0;
    std::string damage_stat = "";
    int stockpile_idx = 0;
	int voxel_model = 0;
	int clothing_layer = 0;
};
