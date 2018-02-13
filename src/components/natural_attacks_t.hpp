#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../raws/species.hpp"
#include "../raws/defs/civilization_t.hpp"
#include "../bengine/ecs_impl.hpp"

struct natural_attack_t {
	natural_attack_t() = default;
    natural_attack_t(const civ_unit_natural_attack_t &na) noexcept {
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

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( type, hit_bonus, n_dice, die_type, die_mod, range );
    }
};

struct natural_attacks_t {

    natural_attacks_t() {}

    std::vector<natural_attack_t> attacks;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( attacks );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<natural_attacks_t>>)
