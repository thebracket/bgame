#pragma once

#include <string>
#include <bitset>
#include <rltk.hpp>
#include <boost/container/flat_map.hpp>
#include <fstream>

constexpr int NUMBER_OF_ITEM_CATEGORIES = 6;
constexpr int COMPONENT = 0;
constexpr int TOOL_CHOPPING = 1;
constexpr int TOOL_DIGGING = 2;
constexpr int WEAPON_MELEE = 3;
constexpr int WEAPON_RANGED = 4;
constexpr int WEAPON_AMMO = 5;

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
    rltk::color_t fg;
    rltk::color_t bg;
    int damage_n=0, damage_d=0, damage_mod=0;
    int range = 0;
    std::string ammo;
    int stack_size = 1;
    int initiative_penalty = 0;
    std::string damage_stat = "";
    int stockpile_idx = 0;
};

extern boost::container::flat_map<std::string, item_def_t> item_defs;
extern boost::container::flat_map<int, stockpile_def_t> stockpile_defs;
extern int clothing_stockpile;

void read_items(std::ofstream &tech_tree_file) noexcept;
void sanity_check_items() noexcept;
void read_stockpiles() noexcept;
void sanity_check_stockpiles() noexcept;

