#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"

struct ai_settler_new_arrival_t {

    ai_settler_new_arrival_t() {}
    int turns_since_arrival = 0;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( turns_since_arrival );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_settler_new_arrival_t>>)
