#pragma once

#include <bitset>
#include <cereal/cereal.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs.hpp"

struct stockpile_t {

    stockpile_t() {}

    std::bitset<128> category;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( category ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<stockpile_t>>)
