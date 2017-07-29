#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct sleep_clock_t {

    sleep_clock_t() {}
    int sleep_requirement = 0;
    bool is_sleeping = false;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( sleep_requirement, is_sleeping );
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<sleep_clock_t>>)
