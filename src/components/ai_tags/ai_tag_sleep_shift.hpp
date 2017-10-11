#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs.hpp"

struct ai_tag_sleep_shift_t {

    ai_tag_sleep_shift_t() {}

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        //archive( );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_sleep_shift_t>>)
