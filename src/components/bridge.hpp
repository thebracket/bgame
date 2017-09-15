#pragma once

#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include "../bengine/ecs_impl.hpp"

struct bridge_t {
    bool complete = false;
    bool retracted = false;

    bridge_t() {}

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( complete, retracted ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<bridge_t>>)
