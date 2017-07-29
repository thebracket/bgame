#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct turret_t {
    turret_t() {}
    turret_t(const int rng, const int hb, const int n, const int d, const int bonus, const std::size_t civ) : range(rng), hit_bonus(hb),
                                                                                       damage_dice(n),
                                                                                       damage_die(d),
                                                                                       damage_bonus(bonus),
                                                                                       owner_civilization(civ) {}

    int range = 6;
    int hit_bonus = 0;
    int damage_dice = 1;
    int damage_die = 6;
    int damage_bonus = 0;
    std::size_t owner_civilization = 0;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( range, hit_bonus, damage_dice, damage_die, damage_bonus, owner_civilization ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<turret_t>>)