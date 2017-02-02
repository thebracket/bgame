#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct slidemove_t {
    float offsetX = 0;
    float offsetY = 0;
    float offsetZ = 0;
    int lifespan = 0;

    slidemove_t() {}
    slidemove_t(const float &x, const float &y, const float &z, const int &life) :
        offsetX(x), offsetY(y), offsetZ(z), lifespan(life) {}

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( offsetX, offsetY, offsetZ, lifespan ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<slidemove_t>>)