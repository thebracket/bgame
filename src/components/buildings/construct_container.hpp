#pragma once

#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include "../../bengine/ecs_impl.hpp"

struct construct_container_t {
    template<class Archive>
    void serialize(Archive & archive)
    {
        //archive(  ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<construct_container_t>>)
