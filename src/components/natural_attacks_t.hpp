#pragma once

#include "../raws/defs/civilization_t.hpp"
#include <vector>

struct natural_attack_t {
	natural_attack_t() = default;
    explicit natural_attack_t(const civ_unit_natural_attack_t &na) noexcept {
        type = na.type;
        hit_bonus = na.hit_bonus;
        n_dice = na.n_dice;
        die_type = na.die_type;
        die_mod = na.die_mod;
        range = na.range;
    }

    std::string type = "";
    int hit_bonus = 0;
    int n_dice = 0;
    int die_type = 6;
    int die_mod = 0;
    int range = 0;    
};

struct natural_attacks_t {

    natural_attacks_t() {}

    std::vector<natural_attack_t> attacks;    
};

