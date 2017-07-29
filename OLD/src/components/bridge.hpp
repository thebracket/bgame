#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

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

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<bridge_t>>)
