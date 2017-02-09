#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct ai_tag_my_turn_t {

    ai_tag_my_turn_t() {}

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        //archive( );
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<ai_tag_my_turn_t>>)
