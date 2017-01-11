#pragma once

#include <string>
#include <vector>
#include <utility>
#include <boost/container/flat_map.hpp>
#include <bitset>
#include "reaction_input.hpp"

constexpr int special_reaction_cooking = 0;
constexpr int special_reaction_tanning = 1;

struct reaction_t {
    std::string tag = "";
    std::string name = "";
    std::string workshop = "";
    std::vector<reaction_input_t> inputs;
    std::vector<std::pair<std::string, int>> outputs;
    std::string skill = "";
    int difficulty = 10;
    bool automatic = false;
    int power_drain = 0;
    bool emits_smoke = false;
    std::bitset<2> specials;
};

extern boost::container::flat_map<std::string, reaction_t> reaction_defs;
extern boost::container::flat_map<std::string, std::vector<std::string>> reaction_building_defs;

void sanity_check_reactions() noexcept;
void read_reactions(std::ofstream &tech_tree_file) noexcept;