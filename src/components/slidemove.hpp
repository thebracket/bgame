#pragma once

#include "../bengine/ecs_impl.hpp"
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

struct slidemove_t {
    float offsetX = 0;
    float offsetY = 0;
    float offsetZ = 0;
    int lifespan = 0;

	slidemove_t() = default;
    slidemove_t(const float &x, const float &y, const float &z, const int &life) noexcept :
        offsetX(x), offsetY(y), offsetZ(z), lifespan(life) {}

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( offsetX, offsetY, offsetZ, lifespan ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<slidemove_t>>)
