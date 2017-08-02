#pragma once

#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"

enum render_mode_t { RENDER_SETTLER, RENDER_SENTIENT };

struct renderable_composite_t {
    render_mode_t render_mode = RENDER_SETTLER;
    uint16_t ascii_char = '@';

    renderable_composite_t() {}
    renderable_composite_t(const render_mode_t &rm) : render_mode(rm) {}
    renderable_composite_t(const render_mode_t &rm, const uint16_t &ascii) : render_mode(rm), ascii_char(ascii) {}

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( render_mode, ascii_char ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<renderable_composite_t>>)