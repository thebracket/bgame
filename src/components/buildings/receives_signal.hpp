#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"

struct receives_signal_t {
    receives_signal_t() {}

    bool active = false;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( active ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<receives_signal_t>>)