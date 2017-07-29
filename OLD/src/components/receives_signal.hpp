#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct receives_signal_t {
    receives_signal_t() {}

    bool active = false;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( active ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<receives_signal_t>>)