#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs.hpp"

struct lever_t {
    bool active = false;
    std::vector<std::size_t> targets;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( active, targets ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<lever_t>>)