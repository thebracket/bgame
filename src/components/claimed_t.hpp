#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs.hpp"

struct claimed_t {

    claimed_t() {}
    claimed_t(const std::size_t &id) : claimed_by(id) {}
    std::size_t claimed_by = 0;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( claimed_by );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<claimed_t>>)
