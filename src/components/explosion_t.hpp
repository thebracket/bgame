#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/unordered_set.hpp>
#include "../bengine/ecs_impl.hpp"

struct explosion_t {
	explosion_t() = default;
    explosion_t(const uint16_t fuse, const uint16_t radius,
                const uint8_t n, const uint16_t d) noexcept : blast_radius(radius), fuse_timer(fuse), damage_dice(n),
                                                     damage_dice_type(d)
    {}

    uint16_t blast_radius = 0;
    uint8_t blast_timer = 254;
    uint16_t fuse_timer = 254;
    uint8_t damage_dice = 1;
    uint16_t damage_dice_type = 6;
    std::unordered_set<int> tiles_hit;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( blast_radius, blast_timer, fuse_timer, damage_dice, damage_dice_type, tiles_hit ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<explosion_t>>)
