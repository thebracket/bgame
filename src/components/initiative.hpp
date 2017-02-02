#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

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

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<initiative_t>>)