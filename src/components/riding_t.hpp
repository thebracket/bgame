#pragma once

#include "../stdafx.h"
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"

struct riding_t {
    std::size_t riding;
    riding_t() {}
    riding_t(const std::size_t carrier) : riding(carrier) {}

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( riding ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<riding_t>>)