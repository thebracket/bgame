#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"

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

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<sleep_clock_t>>)
