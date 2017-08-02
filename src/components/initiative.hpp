#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"

struct initiative_t {
    initiative_t() {}
    initiative_t(const int init_mod) : initiative_modifier(init_mod) {}
    int initiative = 0;
    int initiative_modifier = 0;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( initiative, initiative_modifier ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<initiative_t>>)