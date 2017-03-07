#pragma once

#include <rltk.hpp>
#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

enum render_mode_t { RENDER_SETTLER, RENDER_SENTIENT };

struct renderable_composite_t {
    render_mode_t render_mode = RENDER_SETTLER;

    renderable_composite_t() {}
    renderable_composite_t(const render_mode_t &rm) : render_mode(rm) {}

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( render_mode ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<renderable_composite_t>>)